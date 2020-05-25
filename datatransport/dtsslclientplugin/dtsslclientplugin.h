#ifndef DT__SSLCLIENT__PLUGIN__H
#define DT__SSLCLIENT__PLUGIN__H

#include "datatransportinterface.h"
#include <QSslSocket>
#include <QTimer>

#ifdef DT_SSLCLIENT_PLUGIN_EXPORT
#define DT_SSLCLIENT_PLUGIN_DECL Q_DECL_EXPORT 
#else
#define DT_SSLCLIENT_PLUGIN_DECL Q_DECL_IMPORT 
#endif

class DT_SSLCLIENT_PLUGIN_DECL DtSslClientFactory: public QObject, public DataTransportFactory
{
	Q_OBJECT

	Q_INTERFACES(DataTransportFactory)
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
	Q_PLUGIN_METADATA(IID "tools.datatransportfactory.DataTransportFactory" FILE "dtsslclientfactory.json")
#endif

public:
	virtual ~DtSslClientFactory() {}

	virtual QSharedPointer<QObject> createOneDataTransportInstance( const QVariantHash &parameter );
};

class DT_SSLCLIENT_PLUGIN_DECL DtSslClientPlugin : public DataTransportInterface
{
	Q_OBJECT

private:
	QString cafilename;
	QSslCertificate sslca;
	QString selfaddress;
	int selfport;
	QString peeraddress;
	int peerport;
	int conninterval;
	QSslSocket sslsocket;
	QTimer connTimer;
	bool encryptedState;

	void reConnectPeer();

public:
	DtSslClientPlugin( const QVariantHash &parameter );
	virtual ~DtSslClientPlugin();

public slots:
	void processData();
	void connTimeOut();
	void discardServer();
	void encryptedOk();
	void recvError(QAbstractSocket::SocketError socketError);
	void recvSslErrors(const QList<QSslError> &errors);

public slots:
	virtual void writeData(const QList<QByteArray> &data);
};

#endif
