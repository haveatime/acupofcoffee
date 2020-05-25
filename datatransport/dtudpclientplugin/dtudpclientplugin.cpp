
#include <QDebug>
#include "dtudpclientplugin.h"

QSharedPointer<QObject> DtUdpClientFactory::createOneDataTransportInstance( const QVariantHash &parameter )
{
	return QSharedPointer<QObject>(new DtUdpClientPlugin(parameter));
}

#if QT_VERSION<QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(staticdtudpclientfactory, DtUdpClientFactory)
#endif

DtUdpClientPlugin::DtUdpClientPlugin( const QVariantHash &parameter )
:DataTransportInterface(parameter)
{
	if (getDebug() > 0)
		qDebug() << "DtUdpClientPlugin::DtUdpClientPlugin parameter=" << parameter;

	if (!parameter.contains("selfaddress"))
		selfaddress = "0.0.0.0";
	else
		selfaddress = parameter["selfaddress"].toString();

	selfport = parameter["selfport"].toInt();
	peeraddress = parameter["peeraddress"].toString();
	peerport = parameter["peerport"].toInt();

#if QT_VERSION>QT_VERSION_CHECK(5,0,0)
	bool bindifok = udpsocket.bind(QHostAddress(selfaddress), selfport, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);
#else
	bool bindifok = udpsocket.bind(QHostAddress(selfaddress), selfport, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
#endif
	if (!bindifok)
	{
		qDebug() << "selfaddress=" << selfaddress << "selfport=" << selfport << "bind error=" << udpsocket.errorString();
		return;
	}

	connect(&udpsocket, SIGNAL(readyRead()), this, SLOT(processDatagrams()));
	connect(&udpsocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(processErrors(QAbstractSocket::SocketError)));
}

DtUdpClientPlugin::~DtUdpClientPlugin()
{
	udpsocket.close();
}

void DtUdpClientPlugin::processErrors(QAbstractSocket::SocketError socketError)
{
	qDebug() << "DtUdpClientPlugin::processErrors=" << socketError;
}

void DtUdpClientPlugin::processDatagrams()
{
	QList<QByteArray> recvlist;

	while (udpsocket.hasPendingDatagrams())
	{
		QByteArray data;
		data.resize(udpsocket.pendingDatagramSize());
		udpsocket.readDatagram(data.data(), data.size());

		recvlist.append(data);
	}

	if( recvlist.size() > 0 )
		emit readData(recvlist);
}

void DtUdpClientPlugin::writeData(const QList<QByteArray> &data)
{
	int count = data.size();

	for( int index=0; index<count; index++ )
	{
		const QByteArray &frame = data.at(index);
		udpsocket.writeDatagram(frame.data(), frame.size(), QHostAddress(peeraddress), peerport);
	}
}
