#ifndef DP__IEC104__PLUGIN__H
#define DP__IEC104__PLUGIN__H

#include "dataprotocolinterface.h"

#ifdef DP_IEC104_PLUGIN_EXPORT
#define DP_IEC104_PLUGIN_DECL Q_DECL_EXPORT 
#else
#define DP_IEC104_PLUGIN_DECL Q_DECL_IMPORT 
#endif

class DP_IEC104_PLUGIN_DECL DpIec104Factory : public QObject, public DataProtocolFactory
{
	Q_OBJECT

	Q_INTERFACES(DataProtocolFactory)
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
	Q_PLUGIN_METADATA(IID "tools.dataprotocolfactory.DataProtocolFactory" FILE "dpiec104factory.json")
#endif

public:
	virtual ~DpIec104Factory() {}

	virtual QSharedPointer<QObject> createOneDataProtocolInstance( const QVariantHash &parameter );
};

class DP_IEC104_PLUGIN_DECL DpIec104Plugin : public DataProtocolInterface
{
	Q_OBJECT

private:

public:
	DpIec104Plugin(const QVariantHash &parameter);
	virtual ~DpIec104Plugin();

public slots:
	virtual void feedInds(const QList<QJsonObject> &data);
	virtual void feedBytes(const QList<QByteArray> &data);
};

#endif
