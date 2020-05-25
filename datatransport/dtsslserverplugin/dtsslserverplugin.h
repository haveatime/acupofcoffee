#ifndef DT__SSLSERVER__PLUGIN__H
#define DT__SSLSERVER__PLUGIN__H

#include "datatransportinterface.h"
#include <QTcpServer>
#include <QSslSocket>
#include <QSharedPointer>
#include <QTimer>
#include <QSslKey>

#ifdef DT_SSLSERVER_PLUGIN_EXPORT
#define DT_SSLSERVER_PLUGIN_DECL Q_DECL_EXPORT 
#else
#define DT_SSLSERVER_PLUGIN_DECL Q_DECL_IMPORT 
#endif

class DT_SSLSERVER_PLUGIN_DECL DtSslServerFactory: public QObject, public DataTransportFactory
{
	Q_OBJECT

	Q_INTERFACES(DataTransportFactory)
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
	Q_PLUGIN_METADATA(IID "tools.datatransportfactory.DataTransportFactory" FILE "dtsslserverfactory.json")
#endif

public:
	virtual ~DtSslServerFactory() {}

	virtual QSharedPointer<QObject> createOneDataTransportInstance( const QVariantHash &parameter );
};

class QSslSimpleServer: public QTcpServer
{
private:
	QString keyfilename;
	QString certfilename;
	QString cafilename;

	QSslCertificate sslcert;
	QSslKey sslkey;
	QSslCertificate sslca;

public:
	QSslSimpleServer(const QVariantHash &parameter, QObject *parent = Q_NULLPTR);
	void incomingConnection(qintptr socketDescriptor);	
};

class DT_SSLSERVER_PLUGIN_DECL DtSslServerPlugin : public DataTransportInterface
{
	Q_OBJECT

private:
	QString selfaddress;
	int selfport;
	QString peeraddress;
	int listeninterval;
	QSslSimpleServer sslserver;
	QSslSocket* sslsocket;
	QTimer listentimer;
	bool encryptedState;

	void reListen();

public:
	DtSslServerPlugin( const QVariantHash &parameter );
	virtual ~DtSslServerPlugin();

public slots:
	void gainConnection();
	void readClient();
	void discardClient();
	void listenTimeOut();
	void encryptedOk();
	void recvError(QAbstractSocket::SocketError socketError);
	void recvSslErrors(const QList<QSslError> &errors);

public slots:
	virtual void writeData(const QList<QByteArray> &data);
};

#endif
