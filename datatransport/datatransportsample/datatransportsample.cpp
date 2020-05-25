#include <QDebug>
#include <QCoreApplication>

#include "transportpluginload.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	TransportPluginLoad pluginload;

	return app.exec();
}
