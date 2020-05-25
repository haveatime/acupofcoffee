#include "freelyasyncemitter.h"

#include <QDebug>

#if QT_VERSION>=QT_VERSION_CHECK(5,2,0)
FreelyAsyncEmitter::FreelyAsyncEmitter(QObject *context, std::function<void()> function)
{
	void(FreelyAsyncEmitter::*member)() = &FreelyAsyncEmitter::SingleShotSignal;
	connect(this, member, context, function, Qt::QueuedConnection);
}

#endif

FreelyAsyncEmitter::FreelyAsyncEmitter(QObject *receiver, const char *method)
{
	connect(this, SIGNAL(SingleShotSignal(const QVariantHash &)), receiver, method, Qt::QueuedConnection);
}

void FreelyAsyncEmitter::emitSingleShot()
{
	emit SingleShotSignal();
}

void FreelyAsyncEmitter::emitSingleShot(const QVariantHash &data)
{
	emit SingleShotSignal(data);
}
