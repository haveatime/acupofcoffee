#ifndef SAMPLE__GENERAL__PLUGIN__H
#define SAMPLE__GENERAL__PLUGIN__H

#include <generalplugininterface.h>
#include <QtPlugin>

#include <QObject>
#include <QString>
#include <QStringList>

#ifdef SAMPLE_GENERAL_PLUGIN_EXPORT
#define SAMPLE_GENERAL_PLUGIN_DECL Q_DECL_EXPORT
#else
#define SAMPLE_GENERAL_PLUGIN_DECL Q_DECL_IMPORT
#endif

class SAMPLE_GENERAL_PLUGIN_DECL SampleGeneralFactory : public QObject, public GeneralPluginFactory
{
	Q_OBJECT

		Q_INTERFACES(GeneralPluginFactory)
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
		Q_PLUGIN_METADATA(IID "tools.generalpluginfactory.GeneralPluginFactory" FILE "samplegeneralfactory.json")
#endif

public:
	SampleGeneralFactory();
	virtual ~SampleGeneralFactory();

	virtual QSharedPointer<QObject> createPluginObject(const QString &category)
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
		Q_DECL_OVERRIDE;
#else
		;
#endif

};

class SAMPLE_GENERAL_PLUGIN_DECL SampleGeneralPluginDefault : public GeneralPluginInterface
{
	Q_OBJECT

public:
	SampleGeneralPluginDefault();
	virtual ~SampleGeneralPluginDefault();

	virtual QVariantHash generalCommunication(const QVariantHash &args)
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
		Q_DECL_OVERRIDE;
#else
		;
#endif

	virtual void generalSlot(const QVariantHash &args)
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
		Q_DECL_OVERRIDE;
#else
		;
#endif
};

class SAMPLE_GENERAL_PLUGIN_DECL SampleGeneralPluginTypeA : public GeneralPluginInterface
{
	Q_OBJECT

public:
	SampleGeneralPluginTypeA();
	virtual ~SampleGeneralPluginTypeA();

	virtual QVariantHash generalCommunication(const QVariantHash &args)
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
		Q_DECL_OVERRIDE;
#else
		;
#endif

	virtual void generalSlot(const QVariantHash &args)
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
		Q_DECL_OVERRIDE;
#else
		;
#endif
};

class SAMPLE_GENERAL_PLUGIN_DECL SampleGeneralPluginTypeB : public GeneralPluginInterface
{
	Q_OBJECT

public:
	SampleGeneralPluginTypeB();
	virtual ~SampleGeneralPluginTypeB();

	virtual QVariantHash generalCommunication(const QVariantHash &args)
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
		Q_DECL_OVERRIDE;
#else
		;
#endif

	virtual void generalSlot(const QVariantHash &args)
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
		Q_DECL_OVERRIDE;
#else
		;
#endif
};

#endif

