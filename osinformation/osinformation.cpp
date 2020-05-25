#include <QCoreApplication>
#include <QDebug> 
#include <QSysInfo> 

int main(int argc, char *argv[])
{
	QCoreApplication coreApplication(argc, argv);

#if QT_VERSION>=QT_VERSION_CHECK(5,4,0)
	qDebug()<<"buildAbi="<<QSysInfo::buildAbi();
	qDebug()<<"buildCpuArchitecture="<<QSysInfo::buildCpuArchitecture();
	qDebug()<<"currentCpuArchitecture="<<QSysInfo::currentCpuArchitecture();
	qDebug()<<"kernelType="<<QSysInfo::kernelType();
	qDebug()<<"kernelVersion="<<QSysInfo::kernelVersion();
	qDebug()<<"machineHostName="<<QSysInfo::machineHostName();
	qDebug()<<"prettyProductName="<<QSysInfo::prettyProductName();
	qDebug()<<"productType="<<QSysInfo::productType();
	qDebug()<<"productVersion="<<QSysInfo::productVersion();
#endif

	return coreApplication.exec();
}
