#include <QCoreApplication>

#include "dtpeekeye.h"

int main(int argc, char **argv)
{
	QString filename("dtdaemon.xml");
	QCoreApplication app(argc, argv);

	if (argc > 1)
		filename = argv[1];

	DtPeekEye peekeye(&app,filename);

	return app.exec();
}
