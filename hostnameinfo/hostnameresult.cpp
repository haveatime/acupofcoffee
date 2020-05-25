#include <QDebug>

#include "hostnameresult.h"

void HostnameResult::lookedResult(const QHostInfo &result)
{
	if (QHostInfo::NoError == result.error())
	{
		qDebug() << "HostnameResult::lookedResult hostName=" << result.hostName();
		qDebug() << "HostnameResult::lookedResult address=" << result.addresses();
	}
	else
		qDebug() << "HostnameResult::lookedResult errorString=" << result.errorString();
}
