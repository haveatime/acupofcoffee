#include <QCoreApplication>
#include <QDebug> 
#include <QHostInfo> 

#include "hostnameresult.h"

int main(int argc, char *argv[])
{
	QCoreApplication qtapp(argc, argv);

	QString localdn = QHostInfo::localDomainName();
	qDebug()<<"localdn="<<localdn;

	QString localhn = QHostInfo::localHostName();
	qDebug() << "localhn=" << localhn;

	QHostInfo localinfo = QHostInfo::fromName("dontknow");
	if (QHostInfo::NoError == localinfo.error())
	{
		qDebug() << "localinfo.hostName=" << localinfo.hostName();
		qDebug() << "localinfo.address=" << localinfo.addresses();
	}
	else
		qDebug() << "localinfo.errorString=" << localinfo.errorString();


	HostnameResult hostinfo;
	QHostInfo::lookupHost("www.baidu.com", &hostinfo, SLOT(lookedResult(const QHostInfo &)));

	return qtapp.exec();
}
