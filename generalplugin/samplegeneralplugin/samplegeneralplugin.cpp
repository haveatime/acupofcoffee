
#include <QDebug>
#include "samplegeneralplugin.h"

SampleGeneralFactory::SampleGeneralFactory()
{
	qDebug() << "SampleGeneralFactory is constructing";
}

QSharedPointer<QObject> SampleGeneralFactory::createPluginObject(const QString &category)
{
	if ( category == "typea" )
		return QSharedPointer<QObject>(new SampleGeneralPluginTypeA);
	else if (category == "typeb")
		return QSharedPointer<QObject>(new SampleGeneralPluginTypeB);
	else
		return QSharedPointer<QObject>(new SampleGeneralPluginDefault);
}

SampleGeneralFactory::~SampleGeneralFactory()
{
	qDebug() << "SampleGeneralFactory is destructing";
}

#if QT_VERSION<QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(staticsamplegeneralfactory, SampleGeneralFactory)
#endif

SampleGeneralPluginDefault::SampleGeneralPluginDefault()
{
	qDebug() << "SampleGeneralPluginDefault is constructing";
}

QVariantHash SampleGeneralPluginDefault::generalCommunication(const QVariantHash &args)
{
	QVariantHash result;

	result["description"] = QString::fromLocal8Bit("插件样例默认.");

	return result;
}

void SampleGeneralPluginDefault::generalSlot(const QVariantHash &args)
{
	qDebug() << "SampleGeneralPluginDefault::generalSlot" << "args=" << args;
}

SampleGeneralPluginDefault::~SampleGeneralPluginDefault()
{
	qDebug() << "SampleGeneralPluginDefault is destructing";
}

SampleGeneralPluginTypeA::SampleGeneralPluginTypeA()
{
	qDebug() << "SampleGeneralPluginTypeA is constructing";
}

QVariantHash SampleGeneralPluginTypeA::generalCommunication(const QVariantHash &args)
{
	QVariantHash result;

	result["description"] = QString::fromLocal8Bit("插件样例类型A.");

	return result;
}

void SampleGeneralPluginTypeA::generalSlot(const QVariantHash &args)
{
	qDebug() << "SampleGeneralPluginTypeA::generalSlot" << "args=" << args;
}

SampleGeneralPluginTypeA::~SampleGeneralPluginTypeA()
{
	qDebug() << "SampleGeneralPluginTypeA is destructing";
}

SampleGeneralPluginTypeB::SampleGeneralPluginTypeB()
{
	qDebug() << "SampleGeneralPluginTypeB is constructing";
}

QVariantHash SampleGeneralPluginTypeB::generalCommunication(const QVariantHash &args)
{
	QVariantHash result;

	result["description"] = QString::fromLocal8Bit("插件样例类型B.");

	return result;
}

void SampleGeneralPluginTypeB::generalSlot(const QVariantHash &args)
{
	qDebug() << "SampleGeneralPluginTypeB::generalSlot" << "args=" << args;
}

SampleGeneralPluginTypeB::~SampleGeneralPluginTypeB()
{
	qDebug() << "SampleGeneralPluginTypeB is destructing";
}
