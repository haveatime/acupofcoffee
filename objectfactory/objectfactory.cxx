#include <QDebug>
#include <QCoreApplication>

#include <QTimer>

#include "objectfactory.h"

int main(int argc, char *argv[])
{
	QCoreApplication qtapp(argc, argv);

	ObjectFactory::registerClass<QTimer>();
	QObject *myobj = ObjectFactory::createObject("QTimer");

	qtapp.exec();

	return 0;
}

