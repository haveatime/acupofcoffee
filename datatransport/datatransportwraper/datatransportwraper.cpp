
#include <QDebug>
#include "datatransportinterface.h"
#include "datatransportwraper.h"

DataTransportWraper::DataTransportWraper(const QVariantHash &params)
{
	initial(params);
}

DataTransportWraper::~DataTransportWraper()
{
	destroy();
}

void DataTransportWraper::destroy()
{
	if (dtplugin)
	{
		DataTransportInterface *transportplugin = qobject_cast<DataTransportInterface *>(dtplugin.data());
		disconnect(transportplugin, SIGNAL(readData(const QList<QByteArray> &)), this, SIGNAL(readData(const QList<QByteArray> &)));
	}

	dtplugin.clear();

	if (dtloader.isLoaded())
	{
		if (!dtloader.unload())
		{
			qDebug() << "DataTransportWraper::Stop" << "plugin=" << plugin << "perhaps the other plugin maybe use the same plugin.";
		}
	}
}

void DataTransportWraper::initial(const QVariantHash &params)
{
	name = params["name"].toString();
	desc = params["desc"].toString();
	plugin = params["plugin"].toString();

	dtloader.setFileName(plugin);
	QObject *pluginfactory = dtloader.instance();
	qDebug() << "DataTransportWraper name=" << name <<" dtloader=" << dtloader.fileName();
	DataTransportFactory *transportfactory = qobject_cast<DataTransportFactory *>(pluginfactory);
	if (transportfactory)
	{
		QSharedPointer<QObject> pluginobject = transportfactory->createOneDataTransportInstance(params);
		DataTransportInterface *transportplugin = qobject_cast<DataTransportInterface *>(pluginobject.data());
		if (transportplugin)
		{
			connect(transportplugin, SIGNAL(readData(const QList<QByteArray> &)), this, SIGNAL(readData(const QList<QByteArray> &)));
			dtplugin = pluginobject;
		}
	}
}

void DataTransportWraper::writeData(const QList<QByteArray> &data)
{
	if (dtplugin)
	{
		DataTransportInterface *transportplugin = qobject_cast<DataTransportInterface *>(dtplugin.data());
		transportplugin->writeData(data);
	}
}

QString DataTransportWraper::getName()
{
	QString name("null");

	if (dtplugin)
	{
		DataTransportInterface *transportplugin = qobject_cast<DataTransportInterface *>(dtplugin.data());
		name = transportplugin->getName();
	}

	return name;
}

QString DataTransportWraper::getDesc()
{
	QString desc("null");

	if (dtplugin)
	{
		DataTransportInterface *transportplugin = qobject_cast<DataTransportInterface *>(dtplugin.data());
		desc = transportplugin->getDesc();
	}

	return desc;
}

int DataTransportWraper::getDebug()
{
	int debug = 0;

	if (dtplugin)
	{
		DataTransportInterface *transportplugin = qobject_cast<DataTransportInterface *>(dtplugin.data());
		debug = transportplugin->getDebug();
	}

	return debug;
}

