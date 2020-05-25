#ifndef MULTICAST__WRAP__H
#define MULTICAST__WRAP__H

#include <QObject>
#include <QString>
#include <QUdpSocket>

#ifdef MULTICAST_WRAP_EXPORT
#define MULTICAST_WRAP_DECL Q_DECL_EXPORT
#else
#define MULTICAST_WRAP_DECL Q_DECL_IMPORT
#endif

/********************************************
224.0.0.0-239.255.255.255为多播地址范围
224.0.0.0-224.0.0.255保留给路由协议使用
********************************************/

class MULTICAST_WRAP_DECL MulticastWrap:public QObject
{
	Q_OBJECT

private:
	QString mcaddress;
	int mcport;
	QUdpSocket mcsocket;

public:
	MulticastWrap(const QString &ipaddress, int ipport);
	~MulticastWrap();

signals:
	void receivingData(const QByteArray &data);

public slots:
	void processDatagrams();
	void processErrors(QAbstractSocket::SocketError socketError);
	void sendingData(const QByteArray &data);
};

#endif
