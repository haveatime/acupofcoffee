#ifndef FREELY__ASYNC__EMITTER__H
#define FREELY__ASYNC__EMITTER__H

#include <QObject>
#include <QVariant>

#include <functional>

class FreelyAsyncEmitter : public QObject
{
	Q_OBJECT

public:
#if QT_VERSION>=QT_VERSION_CHECK(5,2,0)
	FreelyAsyncEmitter(QObject *context, std::function<void()> function);
#endif
	FreelyAsyncEmitter(QObject *receiver, const char *method);

	void emitSingleShot();
	void emitSingleShot(const QVariantHash &data);

signals:
	void SingleShotSignal();
	void SingleShotSignal(const QVariantHash &data);
};

#endif
