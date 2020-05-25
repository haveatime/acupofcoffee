#ifndef DATA__TRANSPORT__WRAPER__H
#define DATA__TRANSPORT__WRAPER__H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QSharedPointer>
#include <QPluginLoader>

#ifdef DATA_TRANSPORT_WRAPER_EXPORT
#define DATA_TRANSPORT_WRAPER_DECL Q_DECL_EXPORT 
#else
#define DATA_TRANSPORT_WRAPER_DECL Q_DECL_IMPORT 
#endif

class DATA_TRANSPORT_WRAPER_DECL DataTransportWraper : public QObject
{
	Q_OBJECT

private:
	QString name;
	QString desc;
	QString plugin;

	QPluginLoader dtloader;
	QSharedPointer<QObject> dtplugin;

private:
	void destroy();
	void initial(const QVariantHash &params);

public:
	DataTransportWraper(const QVariantHash &params);
	~DataTransportWraper();

	QString getName();
	QString getDesc();
	int getDebug();

signals:
	void readData(const QList<QByteArray> &data);

public slots:
	void writeData(const QList<QByteArray> &data);
};

#endif
