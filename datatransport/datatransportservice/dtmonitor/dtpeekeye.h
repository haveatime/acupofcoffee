#ifndef DT__PEEKEYE__H
#define DT__PEEKEYE__H

#include <QString>
#include <QSharedPointer>
#include <QFile>

class DtPeekEye: public QObject
{
	Q_OBJECT

private:
	QSharedPointer<QObject> dtdirector;

public slots:
	void handleDirectorData(const QList<QByteArray> &data);

public:
	DtPeekEye(QObject* parent,const QString &filename);
};

#endif
