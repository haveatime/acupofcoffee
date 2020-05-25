
#include <QDebug>
#include <QSharedMemory>
#include <QFile>

#include "memorywrap.h"

MemoryWrap::~MemoryWrap()
{
}

bool MemoryWrap::create(const QVariantHash &args)
{
	bool flag = false;

	if (category.size() > 0)
	{
		qDebug() << "MemoryWrap::create already created category=" << category;
		return flag;
	}

	QString demand_category = args["category"].toString();
	int demand_length = args["length"].toInt();

	if ("share" == demand_category)
	{
		sharekey = args["key"].toString();
		sharemode = args["mode"].toString();
		sharemem = QSharedPointer<QSharedMemory>(new QSharedMemory(sharekey));

		QSharedMemory::AccessMode accessmode;
		if( "readwrite" == sharemode )
		{
			accessmode =  QSharedMemory::ReadWrite;
		}
		else if( "readonly" == sharemode )
		{
			accessmode =  QSharedMemory::ReadOnly;
		}
		else
		{
			qDebug() << "MemoryWrap::create accessmode is wrong! sharemode=" << sharemode;
			return flag;
		}

		bool ifcreated = sharemem->create(demand_length,accessmode);
		if (ifcreated)
		{
			data = sharemem->data();
			flag = true;
		}
		else
		{
			QSharedMemory::SharedMemoryError errorcreate = sharemem->error();
			if (errorcreate == QSharedMemory::AlreadyExists)
			{
				bool ifattached = sharemem->attach(accessmode);
				if (ifattached)
				{
					data = sharemem->data();
					flag = true;
				}
				else
					qDebug() << "MemoryWrap::create share attach error=%s" << sharemem->errorString() << "args=" << args;
			}
			else
				qDebug() << "MemoryWrap::create share create error=%s" << sharemem->errorString() << "args=" << args;
		}
	}
	else if ("map" == demand_category)
	{
		filename = args["filename"].toString();
		mapfile = QSharedPointer<QFile>(new QFile(filename));
		bool ifopened = mapfile->open(QIODevice::ReadWrite);
		if (ifopened)
		{
			mapfile->resize(demand_length);
			data = mapfile->map(0, demand_length);
			flag = true;
			mapfile->close();
		}
		else
			qDebug()<<"MemoryWrap::create map error=%s"<< mapfile->errorString() <<"args=" << args;
	}
	else if ("malloc" == demand_category)
	{
		charheap = QSharedPointer<char>(new char(demand_length));
		data = charheap.data();
		flag = true;
	}
	else
	{
		qDebug()<<"MemoryWrap::create category is wrong! args=" << args;
	}

	if (flag)
	{
		category = demand_category;
		length = demand_length;
	}

	return flag;
}

void* MemoryWrap::address()
{
	return data;
}
