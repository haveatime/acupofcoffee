#include <QDebug>
#include "dtsslclientplugin.h"

#include <QHostAddress>
#include <QSslCipher>
#include <QSslKey>
#include <QCoreApplication>
#include <QFileInfo>

QSharedPointer<QObject> DtSslClientFactory::createOneDataTransportInstance( const QVariantHash &parameter )
{
	return QSharedPointer<QObject>(new DtSslClientPlugin(parameter));
}

#if QT_VERSION<QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(staticdtsslclientfactory, DtSslClientFactory)
#endif

DtSslClientPlugin::DtSslClientPlugin( const QVariantHash &parameter )
:DataTransportInterface(parameter), encryptedState(false)
{
	if( getDebug() > 0 )
		qDebug() << "DtSslClientPlugin::DtSslClientPlugin parameter=" << parameter;

	if (!parameter.contains("selfaddress"))
		selfaddress = "0.0.0.0";
	else
		selfaddress = parameter["selfaddress"].toString();
	selfport = parameter["selfport"].toInt();
	peeraddress = parameter["peeraddress"].toString();
	peerport = parameter["peerport"].toInt();

	conninterval = parameter["conninterval"].toInt();
	if (conninterval < 5000)
		conninterval = 5000;

	QString execpath = QCoreApplication::applicationDirPath();

	QString cafilename = parameter["cafilename"].toString();
	if (cafilename.size() > 0)
	{
		QFileInfo cafileinfo(cafilename);
		if (!cafileinfo.isAbsolute())
			cafilename = execpath + "/" + cafilename;

		if (getDebug() > 0)
			qDebug() << "DtSslClientPlugin::DtSslClientPlugin cafilename=" << cafilename;

		QFile cafile(cafilename);
		if (cafile.open(QIODevice::ReadOnly))
		{
			QSslCertificate sslcaget(cafile.readAll(), QSsl::Pem);
			sslca = sslcaget;
			if (getDebug() > 0)
				qDebug() << "DtSslClientPlugin::DtSslClientPlugin sslca=" << sslca;
			cafile.close();
		}
	}

	connect(&sslsocket, SIGNAL(readyRead()), SLOT(processData()));
	connect(&sslsocket, SIGNAL(disconnected()), SLOT(discardServer()));
	connect(&sslsocket, SIGNAL(encrypted()), SLOT(encryptedOk()));
	connect(&sslsocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(recvError(QAbstractSocket::SocketError)));
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
	connect(&sslsocket, static_cast<void(QSslSocket::*)(const QList<QSslError> &)>(&QSslSocket::sslErrors), this, static_cast<void(DtSslClientPlugin::*)(const QList<QSslError> &)>(&DtSslClientPlugin::recvSslErrors));
#endif
	connect(&connTimer, SIGNAL(timeout()), this, SLOT(connTimeOut()));

	connTimer.start(conninterval);

	reConnectPeer();
}

DtSslClientPlugin::~DtSslClientPlugin()
{
	sslsocket.close();
}

void DtSslClientPlugin::reConnectPeer()
{
	if (getDebug() > 0)
		qDebug() << "DtSslClientPlugin::reConnectPeer.";

	sslsocket.abort();

#if QT_VERSION>QT_VERSION_CHECK(5,0,0)
	bool bindifok = sslsocket.bind(QHostAddress(selfaddress), selfport, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);
	if (!bindifok)
	{
		qDebug() << "selfaddress=" << selfaddress << "selfport=" << selfport << "bind error=" << sslsocket.errorString();
		return;
	}
	sslsocket.setSocketOption(QAbstractSocket::KeepAliveOption, 1);
#endif

	if (!sslca.isNull())
		sslsocket.addCaCertificate(sslca);

	sslsocket.connectToHostEncrypted(peeraddress, peerport);
}

void DtSslClientPlugin::processData()
{
	if (!encryptedState)
	{
		qDebug() << "DtSslClientPlugin::processData encryptedState=" << encryptedState;
		return;
	}

	QList<QByteArray> recvlist;

	QByteArray data = sslsocket.readAll();
	recvlist.append(data);

	if( recvlist.size() > 0 )
		emit readData(recvlist);
}

void DtSslClientPlugin::writeData(const QList<QByteArray> &data)
{
	if (!encryptedState)
	{
		qDebug() << "DtSslClientPlugin::writeData encryptedState=" << encryptedState;
		return;
	}

	int count = data.size();

	for( int index=0; index<count; index++ )
	{
		const QByteArray &frame = data.at(index);
		sslsocket.write(frame);
	}
}

void DtSslClientPlugin::connTimeOut()
{
	if (!encryptedState)
		reConnectPeer();
}

void DtSslClientPlugin::discardServer()
{
	encryptedState = false;

	if (getDebug() > 0)
		qDebug() << "DtSslClientPlugin::discardServer.";
}

void DtSslClientPlugin::encryptedOk()
{
	encryptedState = true;

	QSslSocket::SslMode sslmode = sslsocket.mode();
	QSslCertificate peercertificate = sslsocket.peerCertificate();
	QSslKey sslkey = peercertificate.publicKey();
	QList<QSslCertificate> peercertificatechain = sslsocket.peerCertificateChain();
	int peerverifydepth = sslsocket.peerVerifyDepth();
	QSslSocket::PeerVerifyMode peerverifymode = sslsocket.peerVerifyMode();
	QString peerverifyname = sslsocket.peerVerifyName();
	QSslCipher sessioncipher = sslsocket.sessionCipher();
	QSsl::SslProtocol sessionprotocol = sslsocket.sessionProtocol();

	if (getDebug() > 0)
	{
		qDebug() << "DtSslClientPlugin::encryptedOk.";

		qDebug() << "sslmode=" << sslmode;
		qDebug() << "peercertificate=" << peercertificate;
		qDebug() << "sslkey=" << sslkey;
		qDebug() << "peercertificatechain=" << peercertificatechain;
		qDebug() << "peerverifydepth=" << peerverifydepth;
		qDebug() << "peerverifymode=" << peerverifymode;
		qDebug() << "peerverifyname=" << peerverifyname;
		qDebug() << "sessioncipher=" << sessioncipher;
		qDebug() << "sessionprotocol=" << sessionprotocol;
	}
}

void DtSslClientPlugin::recvError(QAbstractSocket::SocketError socketError)
{
	encryptedState = false;

	if (getDebug() > 0)
		qDebug() << "DtSslClientPlugin::recvError socketError=" << socketError;
}

void DtSslClientPlugin::recvSslErrors(const QList<QSslError> &errors)
{
	encryptedState = false;

	if (getDebug() > 0)
		qDebug() << "DtSslClientPlugin::recvSslErrors errors=" << errors;
}
