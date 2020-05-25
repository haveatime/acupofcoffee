
#include <QString>
#include <QSemaphore>
#include <QSystemSemaphore>
#include <QVariant>
#include <QSharedPointer>

#ifdef SEMAPHORE_WRAP_EXPORT
#define SEMAPHORE_WRAP_DECL Q_DECL_EXPORT
#else
#define SEMAPHORE_WRAP_DECL Q_DECL_IMPORT
#endif

class SEMAPHORE_WRAP_DECL SemaphoreWrap
{
	QString category;
	int value;

	QString syswidekey;
	QString syswidemode;
	QSharedPointer<QSystemSemaphore> syswidesem;

	QSharedPointer<QSemaphore> localsem;

public:
	~SemaphoreWrap();

	bool create(const QVariantHash &args);
	bool acquire(int n);
	bool release(int n);
};
