#ifndef DT__COORDINATOR__H
#define DT__COORDINATOR__H

#include <QString>
#include <QHash>
#include <QSharedPointer>
#include <QFile>
#include <QTextStream>

/**********************************************
第1字节:
最低位指示数据方向,0是上行数据,1是下行数据
次低位指示数据类别,0是报文数据,1是管理数据
其余位保留为0.

第2字节起:
通道标识字符串,\0结尾.
**********************************************/

class DtCoordinator: public QObject
{
	Q_OBJECT

private:
	int debug;

	QHash<QString, QSharedPointer<QObject> > dttransport;

	QSharedPointer<QObject> dtdirector;/* director必须用UDP或组播 */

public slots:
	void handleTransportData(const QList<QByteArray> &data);
	void handleDirectorData(const QList<QByteArray> &data);

public:
	DtCoordinator(QObject* parent);
};

#endif

