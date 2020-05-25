#include <QDebug>
#include <QCoreApplication>

#include "multicasttest.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	MulticastTest mbtest("234.234.238.238", 23438);

	return app.exec();
}
