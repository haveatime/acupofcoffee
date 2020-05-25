#include <QDebug>
#include <QCoreApplication>

#include "messagelogger.h"

int main(int argc, char *argv[])
{
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
	qInstallMessageHandler(messageLogger);
#else
	qInstallMsgHandler(messageLogger);
#endif

	QCoreApplication app(argc, argv);

	qDebug() << "customlog running";

	return app.exec();
}
