#include "usertestclass.h"

#include <QDebug>

UserTestClass::UserTestClass()
{
	/* 采用Qt::DirectConnection, 下一行的emit TestSignal1会立即触发. 而且不需要依赖qtapp.exec执行, 而采用Qt::QueuedConnection, 下一行的emit TestSignal1就是异步触发. */
	connect(this, SIGNAL(TestSignal1(QObject *)), this, SLOT(TestSlot1(QObject *)), Qt::DirectConnection);

	emit TestSignal1(this);
}

UserTestClass::~UserTestClass()
{

}

void UserTestClass::TestSlot1(QObject *obj)
{
	QObject *whosend = sender();
	qDebug() << "TestClass::TestSlot1 obj=" << obj << "whosend=" << whosend;
}

void UserTestClass::TestSlot2()
{
	QObject *whosend = sender();
	qDebug() << "TestClass::TestSlot2" << "whosend=" << whosend;
}
