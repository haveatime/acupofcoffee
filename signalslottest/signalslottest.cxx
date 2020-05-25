#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

#include "usertestclass.h"

int main(int argc, char **argv)
{
	QCoreApplication qtapp(argc, argv);

	{
		/*****************************************************************
		onestaticobj������static, ��һ�е�QTimer::singleShot���ܴ���TestSlot2, ��������qtapp.exec�д���.
		����Ļ�,��onestaticobj��������ʱ,��������Ѿ�posted�������ϵ��¼�.
		*****************************************************************/
		static UserTestClass onestaticobj;

		QTimer::singleShot(0, &onestaticobj, SLOT(TestSlot2()));
	}

	UserTestClass onetestobj;

	QObject *qtobj = new QObject();
	QObject::connect(qtobj, SIGNAL(destroyed(QObject *)), &onetestobj, SLOT(TestSlot1(QObject *)));
	qtobj->deleteLater();

	QTimer Testtimer;
	QObject::connect(&Testtimer, SIGNAL(timeout()), &onetestobj, SLOT(TestSlot2()));
	Testtimer.start(10000);

	qtapp.exec();

    return 0;
}
