#ifndef TRANSPORT__PLUGIN__LOAD__H
#define TRANSPORT__PLUGIN__LOAD__H

#include <QObject>
#include <QTimer>

#include "datatransportinterface.h"

class TransportPluginLoad : public QObject
{
	Q_OBJECT

private:
	QHash<QString, QSharedPointer<QObject> > dtwraperhash;
	QTimer sendtimer;

	void loadTransportPluginConfig();

public:
	TransportPluginLoad();

signals:
	void sendData(const QList<QByteArray> &data);

public slots:
	void timeOut();
	void recvData(const QList<QByteArray> &data);
};

#endif
