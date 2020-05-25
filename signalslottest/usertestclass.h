#ifndef TEST__CLASS__H
#define TEST__CLASS__H

#include <QObject>
#include <QVariant>

class UserTestClass : public QObject
{
	Q_OBJECT

public:
	UserTestClass();
	~UserTestClass();

public slots:
	void TestSlot1(QObject *obj);
	void TestSlot2();

signals:
	void TestSignal1(QObject *obj);

};

#endif
