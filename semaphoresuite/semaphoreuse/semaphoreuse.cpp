#include <QDebug>
#include <QCoreApplication>
#include <QThread>

#include "semaphorewrap.h"

void blockseconds( int seconds )
{
	QThread onethread;

	onethread.start();
	onethread.wait(seconds*1000);

	onethread.quit();
	onethread.wait();
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	QVariantHash semargs;
	semargs["category"] = "system";
	semargs["value"] = "1";
	semargs["key"] = "syskey";
	semargs["mode"] = "create";

/* linux下使用ipcs -si semid查看某个sem信号集的详细信息 */

	SemaphoreWrap semuse;
	bool flag = semuse.create(semargs);

	if (flag)
	{
		semuse.acquire(1);
		qDebug() << "semuse.acquire ok!";

		blockseconds(60);
		qDebug() << "blockseconds ok!";

		semuse.release(1);
		qDebug() << "semuse.release ok!";
	}

	return app.exec();
}
