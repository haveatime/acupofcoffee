#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

#include "usertestclass.h"

int main(int argc, char **argv)
{
	QCoreApplication qtapp(argc, argv);

	{
		/*****************************************************************
		onestaticobj必须是static, 下一行的QTimer::singleShot才能触发TestSlot2, 而且是在qtapp.exec中触发.
		否则的话,在onestaticobj对象析构时,会清除掉已经posted到自身上的事件.
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
