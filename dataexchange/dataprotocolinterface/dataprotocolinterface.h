#ifndef DATA__PROTOCOL__PLUGIN__INTERFACE__H
#define DATA__PROTOCOL__PLUGIN__INTERFACE__H

#include <QObject>
#include <QtPlugin>
#include <QString>
#include <QList>
#include <QSharedPointer>
#include <QByteArray>
#include <QVariant>
#include <QJsonObject>

#ifdef DATA_PROTOCOL_PLUGIN_INTERFACE_EXPORT
#define DATA_PROTOCOL_PLUGIN_INTERFACE_DECL Q_DECL_EXPORT 
#else
#define DATA_PROTOCOL_PLUGIN_INTERFACE_DECL Q_DECL_IMPORT 
#endif

class DATA_PROTOCOL_PLUGIN_INTERFACE_DECL DataProtocolFactory
{
public:
	virtual ~DataProtocolFactory() {}

	virtual QSharedPointer<QObject> createOneDataProtocolInstance( const QVariantHash &parameter ) = 0;
};

class DATA_PROTOCOL_PLUGIN_INTERFACE_DECL DataProtocolInterface : public QObject
{
	Q_OBJECT

	QString name;
	QString desc;
	int debug;

public:
	DataProtocolInterface(const QVariantHash &parameter);
	virtual ~DataProtocolInterface();

	QString getName();
	QString getDesc();
	int getDebug();

signals:
	void rawDataReady(const QList<QJsonObject> &data);
	void framesReady(const QList<QByteArray> &data);

public slots:
	virtual void feedInds(const QList<QJsonObject> &data) = 0;
	virtual void feedBytes(const QList<QByteArray> &data) = 0;
};

Q_DECLARE_INTERFACE(DataProtocolFactory, "tools.dataprotocolfactory.DataProtocolFactory")

#endif
