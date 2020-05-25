#include "freelyasynccallwrap.h"

#include <QDebug>

#if QT_VERSION>=QT_VERSION_CHECK(5,2,0)
void FreelyAsyncCallWrap::SingleShot(QObject *context, std::function<void()> function)
{
	FreelyAsyncEmitter emitter(context,function);

	emitter.emitSingleShot();
}
#endif

void FreelyAsyncCallWrap::SingleShot(QObject *receiver, const char *method, const QVariantHash &data)
{
	FreelyAsyncEmitter emitter(receiver, method);

	emitter.emitSingleShot(data);
}
