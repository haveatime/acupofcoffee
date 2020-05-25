#ifndef DT__TCPCLIENT__PLUGIN__H
#define DT__TCPCLIENT__PLUGIN__H

#include "datatransportinterface.h"
#include <QTcpSocket>
#include <QTimer>

#ifdef DT_TCPCLIENT_PLUGIN_EXPORT
#define DT_TCPCLIENT_PLUGIN_DECL Q_DECL_EXPORT 
#else
#define DT_TCPCLIENT_PLUGIN_DECL Q_DECL_IMPORT 
#endif

class DT_TCPCLIENT_PLUGIN_DECL DtTcpClientFactory: public QObject, public DataTransportFactory
{
	Q_OBJECT

	Q_INTERFACES(DataTransportFactory)
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
	Q_PLUGIN_METADATA(IID "tools.datatransportfactory.DataTransportFactory" FILE "dttcpclientfactory.json")
#endif

public:
	virtual ~DtTcpClientFactory() {}

	virtual QSharedPointer<QObject> createOneDataTransportInstance( const QVariantHash &parameter );
};

class DT_TCPCLIENT_PLUGIN_DECL DtTcpClientPlugin : public DataTransportInterface
{
	Q_OBJECT

private:
	QString selfaddress;
	int selfport;
	QString peeraddress;
	int peerport;
	int conninterval;
	QTcpSocket tcpsocket;
	QTimer conntimer;

	void reConnectPeer();

public:
	DtTcpClientPlugin( const QVariantHash &parameter );
	virtual ~DtTcpClientPlugin();

public slots:
	void processData();
	void connTimeOut();

public slots:
	virtual void writeData(const QList<QByteArray> &data);
};

#endif
