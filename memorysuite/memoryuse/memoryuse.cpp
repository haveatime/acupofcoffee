#include <QDebug>
#include <QCoreApplication>

#include "memorywrap.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	QVariantHash memargs;
	memargs["category"] = "share";
	memargs["key"] = "sharekey";
	memargs["filename"] = "mapfilename";
	memargs["length"] = 1 * 1024 * 1024;
	memargs["mode"] = "readwrite";

/* linux下使用ipcs -mi shmid查看某个shm共享内存的详细信息 */

	MemoryWrap memuse;
	bool flag = memuse.create(memargs);

	if (flag)
	{
		char *p = (char *)memuse.address();

		if( memargs["mode"] == "readwrite" )
		{
			p[0] = '1';
			p[1] = '2';
			p[2] = '3';
		}
	}

	return app.exec();
}
