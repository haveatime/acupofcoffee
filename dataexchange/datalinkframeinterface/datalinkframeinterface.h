#ifndef DATA__LINKFRAME__INTERFACE__H
#define DATA__LINKFRAME__INTERFACE__H

#include <QObject>
#include <QString>
#include <QList>
#include <QByteArray>
#include <QVariant>

#ifdef DATA_LINKFRAME_INTERFACE_EXPORT
#define DATA_LINKFRAME_INTERFACE_DECL Q_DECL_EXPORT 
#else
#define DATA_LINKFRAME_INTERFACE_DECL Q_DECL_IMPORT 
#endif

class DATA_LINKFRAME_INTERFACE_DECL DataLinkframeInterface : public QObject
{
	Q_OBJECT

	QString name;
	QString desc;
	int debug;

public:
	DataLinkframeInterface(const QVariantHash &parameter);
	virtual ~DataLinkframeInterface();

	QString getName();
	QString getDesc();
	int getDebug();

	virtual void restore() = 0;

signals:
	void framesReady(const QList<QByteArray> &data);

public slots:
	virtual void feedBytes(const QList<QByteArray> &data) = 0;
};

#endif
