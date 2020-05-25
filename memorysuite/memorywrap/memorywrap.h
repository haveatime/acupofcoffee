
#include <QString>
#include <QFile>
#include <QSharedMemory>
#include <QVariant>
#include <QSharedPointer>

#ifdef MEMORY_WRAP_EXPORT
#define MEMORY_WRAP_DECL Q_DECL_EXPORT
#else
#define MEMORY_WRAP_DECL Q_DECL_IMPORT
#endif

class MEMORY_WRAP_DECL MemoryWrap
{
	QString category;
	int length;

	QString sharekey;
	QString sharemode;
	QSharedPointer<QSharedMemory> sharemem;

	QString filename;
	QSharedPointer<QFile> mapfile;

	QSharedPointer<char> charheap;

	void *data;

public:
	~MemoryWrap();

	bool create(const QVariantHash &args);
	void* address();
};
