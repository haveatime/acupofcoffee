#include "usertestclass.h"

#include <QDebug>

UserTestClass::UserTestClass()
{
	/* ����Qt::DirectConnection, ��һ�е�emit TestSignal1����������. ���Ҳ���Ҫ����qtapp.execִ��, ������Qt::QueuedConnection, ��һ�е�emit TestSignal1�����첽����. */
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
