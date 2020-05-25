#ifndef FREELY__ASYNC__CALL__H
#define FREELY__ASYNC__CALL__H

#ifdef FREELY_ASYNC_CALL_WRAP_EXPORT
#define FREELY_ASYNC_CALL_WRAP_DECL Q_DECL_EXPORT
#else
#define FREELY_ASYNC_CALL_WRAP_DECL Q_DECL_IMPORT
#endif

#include "freelyasyncemitter.h"

class FREELY_ASYNC_CALL_WRAP_DECL FreelyAsyncCallWrap
{
public:
#if QT_VERSION>=QT_VERSION_CHECK(5,2,0)
	static void SingleShot(QObject *context, std::function<void()> function);
#endif

	static void SingleShot(QObject *receiver, const char *method, const QVariantHash &data);
};

#endif
