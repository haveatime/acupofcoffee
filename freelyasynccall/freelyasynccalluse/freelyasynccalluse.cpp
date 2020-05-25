#include <QDebug>
#include <QCoreApplication>

#include "freelyasynccallwrap.h"
#include "freelycallclass.h"

void FreelyCallFunc(const QVariantHash &data)
{
	qDebug() << "FreelyCallFunc data=" << data;
}

/**********************************************************************************
qtbase\src\corelib\kernel\qobject.cpp:490
void QMetaCallEvent::placeMetaCall(QObject *object)
{
	if (slotObj_) {
		slotObj_->call(object, args_);
	}
	else if (callFunction_ && method_offset_ <= object->metaObject()->methodOffset()) {
		callFunction_(object, QMetaObject::InvokeMetaMethod, method_relative_, args_);
	}
	else {
		QMetaObject::metacall(object, QMetaObject::InvokeMetaMethod, method_offset_ + method_relative_, args_);
	}
}
**********************************************************************************/

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

#if QT_VERSION>=QT_VERSION_CHECK(5,2,0)
	{
		/* target对象提供了FreelyCallFunc函数运行的上下文线程 */
		QObject *target1 = new QObject();
		QVariantHash calldata1;
		calldata1.insert("key1", "data1");

		std::function<void()> function1 = std::bind(FreelyCallFunc, calldata1);
		/* 在上面qobject.cpp代码中的slotObj_处调用 */
		FreelyAsyncCallWrap::SingleShot(target1, function1);
		target1->deleteLater();
	}
#endif

	FreelyCallClass oneobject;
	QVariantHash calldata2;
	calldata2.insert("key2", "data2");

	/* 在上面qobject.cpp代码中的callFunction_处调用 */
	FreelyAsyncCallWrap::SingleShot(&oneobject, SLOT(FreelyCallSlot(const QVariantHash &)), calldata2);

#if QT_VERSION>=QT_VERSION_CHECK(5,2,0)
	QObject *target2 = new QObject();
	std::function<void()> function2 = std::bind(&FreelyCallClass::FreelyCallSlot, &oneobject, calldata2);
	/* 在上面qobject.cpp代码中的slotObj_处调用 */
	FreelyAsyncCallWrap::SingleShot(target2, function2);
	target2->deleteLater();
#endif

	return app.exec();
}
