#ifndef DT__MULTICAST__PLUGIN__H
#define DT__MULTICAST__PLUGIN__H

#include "datatransportinterface.h"
#include <QUdpSocket>

#ifdef DT_MULTICAST_PLUGIN_EXPORT
#define DT_MULTICAST_PLUGIN_DECL Q_DECL_EXPORT 
#else
#define DT_MULTICAST_PLUGIN_DECL Q_DECL_IMPORT 
#endif

class DT_MULTICAST_PLUGIN_DECL DtMultiCastFactory: public QObject, public DataTransportFactory
{
	Q_OBJECT

	Q_INTERFACES(DataTransportFactory)
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
	Q_PLUGIN_METADATA(IID "tools.datatransportfactory.DataTransportFactory" FILE "dtmulticastfactory.json")
#endif

public:
	virtual ~DtMultiCastFactory() {}

	virtual QSharedPointer<QObject> createOneDataTransportInstance( const QVariantHash &parameter );
};

class DT_MULTICAST_PLUGIN_DECL DtMultiCastPlugin : public DataTransportInterface
{
	Q_OBJECT

private:
	QString mcaddress;
	int mcport;
	QUdpSocket mcsocket;
	int loopbackvalue;

public:
	DtMultiCastPlugin( const QVariantHash &parameter );
	virtual ~DtMultiCastPlugin();

public slots:
	void processDatagrams();
	void processErrors(QAbstractSocket::SocketError socketError);

public slots:
	virtual void writeData(const QList<QByteArray> &data);
};

#endif
