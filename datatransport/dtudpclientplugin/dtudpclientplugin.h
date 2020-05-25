#ifndef DT__UDPCLIENT__PLUGIN__H
#define DT__UDPCLIENT__PLUGIN__H

#include "datatransportinterface.h"
#include <QUdpSocket>

#ifdef DT_UDPCLIENT_PLUGIN_EXPORT
#define DT_UDPCLIENT_PLUGIN_DECL Q_DECL_EXPORT 
#else
#define DT_UDPCLIENT_PLUGIN_DECL Q_DECL_IMPORT 
#endif

class DT_UDPCLIENT_PLUGIN_DECL DtUdpClientFactory: public QObject, public DataTransportFactory
{
	Q_OBJECT

	Q_INTERFACES(DataTransportFactory)
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
	Q_PLUGIN_METADATA(IID "tools.datatransportfactory.DataTransportFactory" FILE "dtudpclientfactory.json")
#endif

public:
	virtual ~DtUdpClientFactory() {}

	virtual QSharedPointer<QObject> createOneDataTransportInstance( const QVariantHash &parameter );
};

class DT_UDPCLIENT_PLUGIN_DECL DtUdpClientPlugin : public DataTransportInterface
{
	Q_OBJECT

private:
	QString selfaddress;
	int selfport;
	QString peeraddress;
	int peerport;
	QUdpSocket udpsocket;

public:
	DtUdpClientPlugin( const QVariantHash &parameter );
	virtual ~DtUdpClientPlugin();

public slots:
	void processDatagrams();
	void processErrors(QAbstractSocket::SocketError socketError);

public slots:
	virtual void writeData(const QList<QByteArray> &data);
};

#endif
