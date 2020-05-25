
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QVariant>

#include "datatransportconfigxml.h"
#include "datatransportwraper.h"

#include "transportpluginload.h"

TransportPluginLoad::TransportPluginLoad()
{
	loadTransportPluginConfig();

	foreach(const QSharedPointer<QObject> &onedtwraper, dtwraperhash)
	{
		connect(this, SIGNAL(sendData(const QList<QByteArray> &)), onedtwraper.data(), SLOT(writeData(const QList<QByteArray> &)));
		connect(onedtwraper.data(), SIGNAL(readData(const QList<QByteArray> &)), this, SLOT(recvData(const QList<QByteArray> &)));
	}

	connect(&sendtimer, SIGNAL(timeout()), SLOT(timeOut()));
	sendtimer.start(30000);
}

void TransportPluginLoad::timeOut()
{
	QList<QByteArray> datalist;
	QByteArray frame;
	frame.append("GET index.html HTTP/1.1\r\n");
	frame.append("Connection: keep-alive\r\n");
	frame.append("\r\n");

	datalist.append(frame);
	emit sendData(datalist);
}

void TransportPluginLoad::recvData(const QList<QByteArray> &data)
{
	QObject *whosend = sender();
	DataTransportWraper *onedtwraper = qobject_cast<DataTransportWraper *>(whosend);
	if (onedtwraper)
	{
		qDebug() << "received data" << "name=" << onedtwraper->getName() << "desc=" << onedtwraper->getDesc();
		foreach(const QByteArray &frame, data)
		{
			qDebug() << frame.toHex();
			qDebug() << frame.data();
		}
	}
}

void TransportPluginLoad::loadTransportPluginConfig()
{
	QString execpath = QCoreApplication::applicationDirPath();
	QString appname = QCoreApplication::applicationName();

	QString filename = execpath + "/" + appname + ".xml";
	DataTransportConfigXml dtconfigxml(filename,"transport");
	QVariantList transportlist = dtconfigxml.getSubTagList();

	qDebug() << "transportlist=" << transportlist;

	int transportcount = transportlist.count();
	for (int index = 0; index < transportcount; index++)
	{
		QVariantHash transportconfig = transportlist.at(index).toHash();
		QString transportname = transportconfig["name"].toString();
		QString transportplugin = transportconfig["plugin"].toString();

		QDir plugindir = QFileInfo(transportplugin).dir();
		if (plugindir.isRelative())
			transportplugin = execpath + "/" + transportplugin;

		QSharedPointer<DataTransportWraper> onedtwraper = QSharedPointer<DataTransportWraper>(new DataTransportWraper(transportconfig));
		if (onedtwraper)
			dtwraperhash[transportname] = onedtwraper;
	}
}

