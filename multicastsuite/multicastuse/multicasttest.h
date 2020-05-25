#ifndef MULTICAST__TEST__H
#define MULTICAST__TEST__H

#include <QTimer>

#include "multicastwrap.h"

class MulticastTest :public QObject
{
	Q_OBJECT
		
private:
	QTimer onetimer;
	MulticastWrap multicastwrap;

public:
	MulticastTest(const QString &ipaddress, int ipport);

public slots:
	void recvData(const QByteArray &data);
	void timeOut();

};

#endif
