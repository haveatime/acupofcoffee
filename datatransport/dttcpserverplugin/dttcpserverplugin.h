#ifndef DT__TCPSERVER__PLUGIN__H
#define DT__TCPSERVER__PLUGIN__H

#include "datatransportinterface.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QSharedPointer>
#include <QTimer>

#ifdef DT_TCPSERVER_PLUGIN_EXPORT
#define DT_TCPSERVER_PLUGIN_DECL Q_DECL_EXPORT 
#else
#define DT_TCPSERVER_PLUGIN_DECL Q_DECL_IMPORT 
#endif

class DT_TCPSERVER_PLUGIN_DECL DtTcpServerFactory: public QObject, public DataTransportFactory
{
	Q_OBJECT

	Q_INTERFACES(DataTransportFactory)
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
	Q_PLUGIN_METADATA(IID "tools.datatransportfactory.DataTransportFactory" FILE "dttcpserverfactory.json")
#endif

public:
	virtual ~DtTcpServerFactory() {}

	virtual QSharedPointer<QObject> createOneDataTransportInstance( const QVariantHash &parameter );
};

class DT_TCPSERVER_PLUGIN_DECL DtTcpServerPlugin : public DataTransportInterface
{
	Q_OBJECT

private:
	QString selfaddress;
	int selfport;
	QString peeraddress;
	int checkinterval;
	QTcpServer tcpserver;
	QTcpSocket* tcpsocket;
	QTimer checktimer;

	void reListen();

public:
	DtTcpServerPlugin( const QVariantHash &parameter );
	virtual ~DtTcpServerPlugin();

public slots:
	void gainConnection();
	void readClient();
	void discardClient();
	void checkTimeOut();

public slots:
	virtual void writeData(const QList<QByteArray> &data);
};

#endif
