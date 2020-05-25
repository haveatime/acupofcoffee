
#include <QDebug>
#include "dtmulticastplugin.h"

QSharedPointer<QObject> DtMultiCastFactory::createOneDataTransportInstance( const QVariantHash &parameter )
{
	return QSharedPointer<QObject>(new DtMultiCastPlugin(parameter));
}

#if QT_VERSION<QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(staticdtmulticastfactory, DtMultiCastFactory)
#endif

DtMultiCastPlugin::DtMultiCastPlugin( const QVariantHash &parameter )
:DataTransportInterface(parameter)
{
	if (getDebug() > 0)
		qDebug() << "DtMultiCastPlugin::DtMultiCastPlugin parameter=" << parameter;

	mcaddress = parameter["mcaddress"].toString();
	mcport = parameter["mcport"].toInt();
	loopbackvalue = parameter["loopbackvalue"].toInt();

#if QT_VERSION>QT_VERSION_CHECK(5,0,0)
	if (!QHostAddress(mcaddress).isMulticast())
	{
		qDebug() << "DtMultiCastPlugin mcaddress=" << mcaddress << "isnt multicast!";
		return;
	}
#endif

/********************************************************
 1 Linux3.9�汾���鲥socket��bindʱ�Ƽ�ʹ��SO_REUSEPORTѡ��,������Ϣ����strace -e bind �������̺�鿴.
 2 bind�˿�����ʱ,�����������̵��û�Ȩ�����.
 3 bindʱ����ʹ��QHostAddress::Broadcast���ع㲥��ַ,������ʹ��QHostAddress::LocalHost���ػػ���ַ.
********************************************************/
#if QT_VERSION>QT_VERSION_CHECK(5,0,0)
	bool bindifok = mcsocket.bind(QHostAddress::AnyIPv4, mcport, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);
#else
	bool bindifok = mcsocket.bind(QHostAddress::Any, mcport, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
#endif
	if (!bindifok)
	{
		qDebug() << "mcaddress=" << mcaddress << "mcport=" << mcport << "bind error=" << mcsocket.errorString();
		return;
	}

	mcsocket.joinMulticastGroup(QHostAddress(mcaddress));
	mcsocket.setSocketOption(QAbstractSocket::MulticastTtlOption, 1);
	mcsocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption, loopbackvalue);

	connect(&mcsocket, SIGNAL(readyRead()), this, SLOT(processDatagrams()));
	connect(&mcsocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(processErrors(QAbstractSocket::SocketError)));
}

DtMultiCastPlugin::~DtMultiCastPlugin()
{
	mcsocket.close();
}

void DtMultiCastPlugin::processErrors(QAbstractSocket::SocketError socketError)
{
	qDebug() << "DtMultiCastPlugin::processErrors=" << socketError;
}

void DtMultiCastPlugin::processDatagrams()
{
	QList<QByteArray> recvlist;

	while (mcsocket.hasPendingDatagrams())
	{
		QByteArray data;
		data.resize(mcsocket.pendingDatagramSize());
		mcsocket.readDatagram(data.data(), data.size());

		recvlist.append(data);
	}

	if( recvlist.size() > 0 )
		emit readData(recvlist);
}

void DtMultiCastPlugin::writeData(const QList<QByteArray> &data)
{
	int count = data.size();

	for( int index=0; index<count; index++ )
	{
		const QByteArray &frame = data.at(index);
		mcsocket.writeDatagram(frame.data(), frame.size(), QHostAddress(mcaddress), mcport);
	}
}