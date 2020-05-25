#include <QCoreApplication> 
#include <QHostInfo>
#include <QDateTime> 
#include <QDebug> 

#include "multicasttest.h"

MulticastTest::MulticastTest(const QString &ipaddress, int ipport)
	:onetimer(this), multicastwrap(ipaddress, ipport)
{
	connect(&onetimer, SIGNAL(timeout()), this, SLOT(timeOut()));
	connect(&multicastwrap, SIGNAL(receivingData(const QByteArray &)), this, SLOT(recvData(const QByteArray &)));

	onetimer.start(30000);
}

void MulticastTest::recvData(const QByteArray &data)
{
	qDebug() << "recvData=" << data;
}

void MulticastTest::timeOut()
{
	QByteArray data;
	QString hostname = QHostInfo::localHostName();
	qint64 pid = QCoreApplication::applicationPid();
	qint64 millisec = QDateTime::currentMSecsSinceEpoch();

	data.append("running on ");
	data.append(hostname);
	data.append(", pid=");
	data.append(QByteArray::number(pid));
	data.append(", millisec=");
	data.append(QByteArray::number(millisec));
	data.append(".");

	multicastwrap.sendingData(data);

	qDebug() << "sendData=" << data;
}
