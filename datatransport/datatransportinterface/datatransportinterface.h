#ifndef DATA__TRANSPORT__PLUGIN__INTERFACE__H
#define DATA__TRANSPORT__PLUGIN__INTERFACE__H

#include <QObject>
#include <QtPlugin>
#include <QString>
#include <QList>
#include <QSharedPointer>
#include <QByteArray>
#include <QVariant>

#ifdef DATA_TRANSPORT_PLUGIN_INTERFACE_EXPORT
#define DATA_TRANSPORT_PLUGIN_INTERFACE_DECL Q_DECL_EXPORT 
#else
#define DATA_TRANSPORT_PLUGIN_INTERFACE_DECL Q_DECL_IMPORT 
#endif

class DATA_TRANSPORT_PLUGIN_INTERFACE_DECL DataTransportFactory
{
public:
	virtual ~DataTransportFactory() {}

	virtual QSharedPointer<QObject> createOneDataTransportInstance( const QVariantHash &parameter ) = 0;
};

class DATA_TRANSPORT_PLUGIN_INTERFACE_DECL DataTransportInterface : public QObject
{
	Q_OBJECT

	QString name;
	QString desc;
	int debug;

public:
	DataTransportInterface(const QVariantHash &parameter);
	virtual ~DataTransportInterface();

	QString getName();
	QString getDesc();
	int getDebug();

signals:
	void readData(const QList<QByteArray> &data);

public slots:
	virtual void writeData(const QList<QByteArray> &data) = 0;
};

Q_DECLARE_INTERFACE(DataTransportFactory, "tools.datatransportfactory.DataTransportFactory")

#endif
