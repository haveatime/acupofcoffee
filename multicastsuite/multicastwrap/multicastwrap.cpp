#include <QDebug>

#include "multicastwrap.h"

MulticastWrap::MulticastWrap(const QString &ipaddress, int ipport)
	:mcaddress(ipaddress), mcport(ipport)
{
	if (!QHostAddress(mcaddress).isMulticast())
	{
		qDebug() << "MulticastWrap ipaddress=" << ipaddress << "isnt multicast!";
		return;
	}

#if QT_VERSION>QT_VERSION_CHECK(5,0,0)
	mcsocket.bind(QHostAddress::AnyIPv4, mcport, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);
#else
	mcsocket.bind(QHostAddress::Any, mcport, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
#endif
	mcsocket.joinMulticastGroup(QHostAddress(mcaddress));
	mcsocket.setSocketOption(QAbstractSocket::MulticastTtlOption, 1);
	mcsocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption, 1);

	connect(&mcsocket, SIGNAL(readyRead()), this, SLOT(processDatagrams()));
	connect(&mcsocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(processErrors(QAbstractSocket::SocketError)));
}

MulticastWrap::~MulticastWrap()
{
		
}

void MulticastWrap::processErrors(QAbstractSocket::SocketError socketError)
{
	qDebug() << "MulticastWrap::processErrors=" << mcsocket.errorString();
}

void MulticastWrap::processDatagrams()
{
	while (mcsocket.hasPendingDatagrams())
	{
		QByteArray data;
		data.resize(mcsocket.pendingDatagramSize());
		mcsocket.readDatagram(data.data(), data.size());
		emit receivingData(data);
	}
}

void MulticastWrap::sendingData(const QByteArray &data)
{
	mcsocket.writeDatagram(data, data.size(), QHostAddress(mcaddress), mcport);
}
