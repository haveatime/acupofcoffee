#include <QDebug>
#include "dttcpserverplugin.h"

#include <QHostAddress>

QSharedPointer<QObject> DtTcpServerFactory::createOneDataTransportInstance( const QVariantHash &parameter )
{
	return QSharedPointer<QObject>(new DtTcpServerPlugin(parameter));
}

#if QT_VERSION<QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(staticdttcpserverfactory, DtTcpServerFactory)
#endif

DtTcpServerPlugin::DtTcpServerPlugin( const QVariantHash &parameter )
:tcpsocket(NULL),DataTransportInterface(parameter)
{
	if (getDebug() > 0)
		qDebug() << "DtTcpServerPlugin::DtTcpServerPlugin parameter=" << parameter;

	if (!parameter.contains("selfaddress"))
		selfaddress = "0.0.0.0";
	else
		selfaddress = parameter["selfaddress"].toString();
	selfport = parameter["selfport"].toInt();
	if (!parameter.contains("peeraddress"))
		peeraddress = "0.0.0.0";
	else
		peeraddress = parameter["peeraddress"].toString();

	checkinterval = parameter["checkinterval"].toInt();
	if (checkinterval < 3000)
		checkinterval = 3000;

	connect(&tcpserver, SIGNAL(newConnection()), this, SLOT(gainConnection()));
	tcpserver.setMaxPendingConnections(1);

	connect(&checktimer, SIGNAL(timeout()), this, SLOT(checkTimeOut()));

	reListen();
}

DtTcpServerPlugin::~DtTcpServerPlugin()
{
	if (tcpsocket)
	{
		tcpsocket->deleteLater();
		tcpsocket=NULL;
	}
	tcpserver.close();
}

void DtTcpServerPlugin::gainConnection()
{
	tcpsocket = tcpserver.nextPendingConnection();
	if (tcpsocket)
	{
		QString clientaddr = tcpsocket->peerAddress().toString();
		if (peeraddress != "0.0.0.0")
		{
			if (clientaddr != peeraddress)
			{
				tcpsocket->deleteLater();
				tcpsocket=NULL;
				return;
			}
		}

		connect(tcpsocket, SIGNAL(readyRead()), SLOT(readClient()));
		connect(tcpsocket, SIGNAL(disconnected()), SLOT(discardClient()));

		tcpserver.close();
		checktimer.start(checkinterval);
	}
}

void DtTcpServerPlugin::discardClient()
{
	tcpsocket->deleteLater();
	tcpsocket=NULL;
}

void DtTcpServerPlugin::readClient()
{
	if (tcpsocket == NULL)
		return;

	if (QAbstractSocket::ConnectedState != tcpsocket->state())
	{
		qDebug() << "DtTcpServerPlugin::readClient not QAbstractSocket::ConnectedState";
		tcpsocket->deleteLater();
		tcpsocket=NULL;
		return;
	}

	QList<QByteArray> recvlist;

	QByteArray data = tcpsocket->readAll();
	recvlist.append(data);

	if( recvlist.size() > 0 )
		emit readData(recvlist);
}

void DtTcpServerPlugin::writeData(const QList<QByteArray> &data)
{
	if (tcpsocket == NULL)
		return;

	if (QAbstractSocket::ConnectedState != tcpsocket->state())
	{
		qDebug() << "DtTcpServerPlugin::writeData not QAbstractSocket::ConnectedState";
		tcpsocket->deleteLater();
		tcpsocket=NULL;
		return;
	}

	int count = data.size();

	for( int index=0; index<count; index++ )
	{
		const QByteArray &frame = data.at(index);
		tcpsocket->write(frame);
	}
}

void DtTcpServerPlugin::reListen()
{
	if (tcpserver.isListening())
		return;

	bool listenifok = tcpserver.listen(QHostAddress(selfaddress), selfport);
	if (!listenifok)
	{
		qDebug() << "selfaddress=" << selfaddress << "selfport=" << selfport << "listen error=" << tcpserver.errorString();

		checktimer.start(checkinterval);
	}
}

void DtTcpServerPlugin::checkTimeOut()
{
	if (tcpsocket && QAbstractSocket::ConnectedState != tcpsocket->state())
	{
		tcpsocket->deleteLater();
		tcpsocket=NULL;
	}

	if (tcpsocket == NULL )
	{
		reListen();

		if (tcpserver.isListening())
			checktimer.stop();
	}
}
