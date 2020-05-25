#ifndef DT__COORDINATOR__H
#define DT__COORDINATOR__H

#include <QString>
#include <QHash>
#include <QSharedPointer>
#include <QFile>
#include <QTextStream>

/**********************************************
��1�ֽ�:
���λָʾ���ݷ���,0����������,1����������
�ε�λָʾ�������,0�Ǳ�������,1�ǹ�������
����λ����Ϊ0.

��2�ֽ���:
ͨ����ʶ�ַ���,\0��β.
**********************************************/

class DtCoordinator: public QObject
{
	Q_OBJECT

private:
	int debug;

	QHash<QString, QSharedPointer<QObject> > dttransport;

	QSharedPointer<QObject> dtdirector;/* director������UDP���鲥 */

public slots:
	void handleTransportData(const QList<QByteArray> &data);
	void handleDirectorData(const QList<QByteArray> &data);

public:
	DtCoordinator(QObject* parent);
};

#endif

