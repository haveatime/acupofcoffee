#include <QDebug>
#include "dttcpclientplugin.h"

#include <QHostAddress>

QSharedPointer<QObject> DtTcpClientFactory::createOneDataTransportInstance( const QVariantHash &parameter )
{
	return QSharedPointer<QObject>(new DtTcpClientPlugin(parameter));
}

#if QT_VERSION<QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(staticdttcpclientfactory, DtTcpClientFactory)
#endif

DtTcpClientPlugin::DtTcpClientPlugin( const QVariantHash &parameter )
:DataTransportInterface(parameter)
{
	if( getDebug() > 0 )
		qDebug() << "DtTcpClientPlugin::DtTcpClientPlugin parameter=" << parameter;

	if (!parameter.contains("selfaddress"))
		selfaddress = "0.0.0.0";
	else
		selfaddress = parameter["selfaddress"].toString();
	selfport = parameter["selfport"].toInt();
	peeraddress = parameter["peeraddress"].toString();
	peerport = parameter["peerport"].toInt();

	conninterval = parameter["conninterval"].toInt();
	if (conninterval < 3000)
		conninterval = 3000;

#if QT_VERSION>QT_VERSION_CHECK(5,0,0)
	bool bindifok = tcpsocket.bind(QHostAddress(selfaddress), selfport, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);
	if (!bindifok)
	{
		qDebug() << "selfaddress=" << selfaddress << "selfport=" << selfport << "bind error=" << tcpsocket.errorString();
		return;
	}
	tcpsocket.setSocketOption(QAbstractSocket::KeepAliveOption, 1);
#endif

	connect(&tcpsocket, SIGNAL(readyRead()), SLOT(processData()));

	reConnectPeer();
}

DtTcpClientPlugin::~DtTcpClientPlugin()
{
	tcpsocket.close();
}

void DtTcpClientPlugin::reConnectPeer()
{
	tcpsocket.connectToHost(peeraddress, peerport);

	if ( !tcpsocket.waitForConnected(1) )
		conntimer.singleShot(conninterval, this, SLOT(connTimeOut()));
}

void DtTcpClientPlugin::processData()
{
	if (QAbstractSocket::ConnectedState != tcpsocket.state())
	{
		qDebug() << "DtTcpClientPlugin::processData not QAbstractSocket::ConnectedState";
		return;
	}

	QList<QByteArray> recvlist;

	QByteArray data = tcpsocket.readAll();
	recvlist.append(data);

	if( recvlist.size() > 0 )
		emit readData(recvlist);
}

void DtTcpClientPlugin::writeData(const QList<QByteArray> &data)
{
	if (QAbstractSocket::ConnectedState != tcpsocket.state())
	{
		qDebug() << "DtTcpClientPlugin::writeData not QAbstractSocket::ConnectedState";
		return;
	}

	int count = data.size();

	for( int index=0; index<count; index++ )
	{
		const QByteArray &frame = data.at(index);
		tcpsocket.write(frame);
	}
}

void DtTcpClientPlugin::connTimeOut()
{
	if ( QAbstractSocket::ConnectedState != tcpsocket.state() )
		reConnectPeer();
}

