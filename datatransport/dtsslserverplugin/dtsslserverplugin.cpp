#include <QDebug>
#include "dtsslserverplugin.h"

#include <QHostAddress>
#include <QCoreApplication>
#include <QFileInfo>

QSharedPointer<QObject> DtSslServerFactory::createOneDataTransportInstance( const QVariantHash &parameter )
{
	return QSharedPointer<QObject>(new DtSslServerPlugin(parameter));
}

#if QT_VERSION<QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(staticdtlssserverfactory, DtSslServerFactory)
#endif

QSslSimpleServer::QSslSimpleServer(const QVariantHash &parameter, QObject *parent)
	:QTcpServer(parent)
{
	int debugflag = parameter["debug"].toInt();

	QString execpath = QCoreApplication::applicationDirPath();

	QString keyfilename = parameter["keyfilename"].toString();
	if (keyfilename.size() > 0)
	{
		QFileInfo keyfileinfo(keyfilename);
		if (!keyfileinfo.isAbsolute())
			keyfilename = execpath + "/" + keyfilename;

		if (debugflag > 0)
			qDebug() << "DtSslServerPlugin::DtSslServerPlugin keyfilename=" << keyfilename;

		QFile keyfile(keyfilename);
		if (keyfile.open(QIODevice::ReadOnly))
		{
			QSslKey sslkeyget(keyfile.readAll(), QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey);
			sslkey = sslkeyget;
			if (debugflag > 0)
				qDebug() << "DtSslServerPlugin::DtSslServerPlugin sslkey=" << sslkey;
			keyfile.close();
		}
	}

	QString certfilename = parameter["certfilename"].toString();
	if (certfilename.size() > 0)
	{
		QFileInfo certfileinfo(certfilename);
		if (!certfileinfo.isAbsolute())
			certfilename = execpath + "/" + certfilename;

		if (debugflag > 0)
			qDebug() << "DtSslServerPlugin::DtSslServerPlugin certfilename=" << certfilename;

		QFile certfile(certfilename);
		if (certfile.open(QIODevice::ReadOnly))
		{
			QSslCertificate sslcertget(certfile.readAll(), QSsl::Der);
			sslcert = sslcertget;
			if (debugflag > 0)
				qDebug() << "DtSslServerPlugin::DtSslServerPlugin sslcert=" << sslcert;
			certfile.close();
		}
	}

	QString cafilename = parameter["cafilename"].toString();
	if (cafilename.size() > 0)
	{
		QFileInfo cafileinfo(cafilename);
		if (!cafileinfo.isAbsolute())
			cafilename = execpath + "/" + cafilename;

		if (debugflag > 0)
			qDebug() << "DtSslServerPlugin::DtSslServerPlugin cafilename=" << cafilename;

		QFile cafile(cafilename);
		if (cafile.open(QIODevice::ReadOnly))
		{
			QSslCertificate sslcaget(cafile.readAll(), QSsl::Pem);
			sslca = sslcaget;
			if (debugflag > 0)
				qDebug() << "DtSslServerPlugin::DtSslServerPlugin sslca=" << sslca;
			cafile.close();
		}
	}
}

void QSslSimpleServer::incomingConnection(qintptr socketDescriptor)
{
	QSslSocket *serverSocket = new QSslSocket;
	if (serverSocket->setSocketDescriptor(socketDescriptor))
	{
		if (!sslcert.isNull())
			serverSocket->setLocalCertificate(sslcert);
		if (!sslkey.isNull())
			serverSocket->setPrivateKey(sslkey);
		if (!sslca.isNull())
			serverSocket->addCaCertificate(sslca);

		addPendingConnection(serverSocket);
		serverSocket->startServerEncryption();
	}
	else
	{
		delete serverSocket;
	}
}

DtSslServerPlugin::DtSslServerPlugin( const QVariantHash &parameter )
	:sslserver(parameter), sslsocket(NULL), DataTransportInterface(parameter), encryptedState(false)
{
	if (getDebug() > 0)
		qDebug() << "DtSslServerPlugin::DtSslServerPlugin parameter=" << parameter;

	if (!parameter.contains("selfaddress"))
		selfaddress = "0.0.0.0";
	else
		selfaddress = parameter["selfaddress"].toString();
	selfport = parameter["selfport"].toInt();
	if (!parameter.contains("peeraddress"))
		peeraddress = "0.0.0.0";
	else
		peeraddress = parameter["peeraddress"].toString();

	listeninterval = parameter["checkinterval"].toInt();
	if (listeninterval < 3000)
		listeninterval = 3000;

	connect(&sslserver, SIGNAL(newConnection()), this, SLOT(gainConnection()));
	sslserver.setMaxPendingConnections(1);

	connect(&listentimer, SIGNAL(timeout()), this, SLOT(listenTimeOut()));

	listentimer.start(listeninterval);

	reListen();
}

DtSslServerPlugin::~DtSslServerPlugin()
{
	if (sslsocket)
	{
		sslsocket->deleteLater();
		sslsocket=NULL;
	}

	sslserver.close();
}

void DtSslServerPlugin::gainConnection()
{
	QTcpSocket *tcpsocket = sslserver.nextPendingConnection();

	sslsocket = qobject_cast<QSslSocket *>(tcpsocket);
	if (sslsocket)
	{
		QString clientaddr = sslsocket->peerAddress().toString();
		if (peeraddress != "0.0.0.0")
		{
			if (clientaddr != peeraddress)
			{
				sslsocket->deleteLater();
				sslsocket=NULL;
				return;
			}
		}

		connect(sslsocket, SIGNAL(readyRead()), SLOT(readClient()));
		connect(sslsocket, SIGNAL(disconnected()), SLOT(discardClient()));
		connect(sslsocket, SIGNAL(encrypted()), SLOT(encryptedOk()));
		connect(sslsocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(recvError(QAbstractSocket::SocketError)));
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
		connect(sslsocket, static_cast<void(QSslSocket::*)(const QList<QSslError> &)>(&QSslSocket::sslErrors), this, static_cast<void(DtSslServerPlugin::*)(const QList<QSslError> &)>(&DtSslServerPlugin::recvSslErrors));
#endif

		sslserver.close();
	}
	else
	{
		if (tcpsocket)
			tcpsocket->deleteLater();
	}
}

void DtSslServerPlugin::encryptedOk()
{
	encryptedState = true;

	if (getDebug() > 0)
		qDebug() << "DtSslServerPlugin::encryptedOk.";
}

void DtSslServerPlugin::discardClient()
{
	encryptedState = false;

	if (getDebug() > 0)
		qDebug() << "DtSslServerPlugin::discardClient.";

	if (sslsocket)
	{
		sslsocket->deleteLater();
		sslsocket = NULL;
	}
}

void DtSslServerPlugin::readClient()
{
	if (sslsocket == NULL)
		return;

	if (!encryptedState)
	{
		qDebug() << "DtSslServerPlugin::readClient encryptedState=" << encryptedState;
		return;
	}

	QList<QByteArray> recvlist;

	QByteArray data = sslsocket->readAll();
	recvlist.append(data);

	if( recvlist.size() > 0 )
		emit readData(recvlist);
}

void DtSslServerPlugin::writeData(const QList<QByteArray> &data)
{
	if (sslsocket == NULL)
		return;

	if (!encryptedState)
	{
		qDebug() << "DtSslServerPlugin::writeData encryptedState=" << encryptedState;
		return;
	}

	int count = data.size();

	for( int index=0; index<count; index++ )
	{
		const QByteArray &frame = data.at(index);
		sslsocket->write(frame);
	}
}

void DtSslServerPlugin::reListen()
{
	if (sslserver.isListening())
		return;

	if (sslsocket)
		return;

	if (getDebug() > 0)
		qDebug() << "DtSslServerPlugin::reListen.";

	bool listenifok = sslserver.listen(QHostAddress(selfaddress), selfport);
	if (!listenifok)
	{
		qDebug() << "selfaddress=" << selfaddress << "selfport=" << selfport << "listen error=" << sslserver.errorString();
	}
}

void DtSslServerPlugin::listenTimeOut()
{
	reListen();
}

void DtSslServerPlugin::recvError(QAbstractSocket::SocketError socketError)
{
	encryptedState = false;

	if (getDebug() > 0)
		qDebug() << "DtSslServerPlugin::recvError socketError=" << socketError;

	if (sslsocket)
	{
		sslsocket->deleteLater();
		sslsocket = NULL;
	}
}

void DtSslServerPlugin::recvSslErrors(const QList<QSslError> &errors)
{
	encryptedState = false;

	if (getDebug() > 0)
		qDebug() << "DtSslServerPlugin::recvSslErrors errors=" << errors;

	if (sslsocket)
	{
		sslsocket->deleteLater();
		sslsocket = NULL;
	}
}
