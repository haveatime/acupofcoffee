#ifndef GENERAL__PLUGIN__INTERFACE__H
#define GENERAL__PLUGIN__INTERFACE__H

#include <QObject>
#include <QtPlugin>
#include <QVariant>
#include <QSharedPointer>

QT_BEGIN_NAMESPACE
class QString;
class QStringList;
QT_END_NAMESPACE

#ifdef GENERAL_PLUGIN_INTERFACE_EXPORT
#define GENERAL_PLUGIN_INTERFACE_DECL Q_DECL_EXPORT
#else
#define GENERAL_PLUGIN_INTERFACE_DECL Q_DECL_IMPORT
#endif

class GENERAL_PLUGIN_INTERFACE_DECL GeneralPluginFactory
{
public:
    virtual ~GeneralPluginFactory() {}

    virtual QSharedPointer<QObject> createPluginObject( const QString &category = "default"  ) = 0;
};

class GENERAL_PLUGIN_INTERFACE_DECL GeneralPluginInterface : public QObject
{
    Q_OBJECT

public:
    virtual ~GeneralPluginInterface() {}

    virtual QVariantHash generalCommunication( const QVariantHash &args ) = 0;

signals:
    void generalSignal(const QVariantHash &args);

public slots:
    virtual void generalSlot(const QVariantHash &args) = 0;

};

QT_BEGIN_NAMESPACE

Q_DECLARE_INTERFACE(GeneralPluginFactory, "tools.generalpluginfactory.GeneralPluginFactory")

QT_END_NAMESPACE

#endif
