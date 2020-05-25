#include <QDebug>
#include <QCoreApplication>
#include <QVariant>
#include <QDir>
#include <QPluginLoader>

#include "generalplugininterface.h"

#ifndef Q_OS_WIN32
extern "C"
{
}
#endif

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	QDir pluginsDir = QDir(qApp->applicationDirPath());

	foreach(QString fileName, pluginsDir.entryList(QDir::Files))
	{
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
		QObject *pluginfactory = loader.instance();
		if (0==pluginfactory)
			qDebug() << "fileName=" << fileName << "error=" << loader.errorString();

		GeneralPluginFactory *onefactory = qobject_cast<GeneralPluginFactory *>(pluginfactory);
		if (onefactory)
		{
			QSharedPointer<QObject> plugintypea = onefactory->createPluginObject("typea");
			GeneralPluginInterface *typeainterface = qobject_cast<GeneralPluginInterface *>(plugintypea.data());
			if (typeainterface)
			{
				QVariantHash args;
				qDebug() << "fileName=" << fileName << "generalCommunication=" << typeainterface->generalCommunication(args);
				plugintypea.clear();
			}

			QSharedPointer<QObject> plugintypeb = onefactory->createPluginObject("typeb");
			GeneralPluginInterface *typebinterface = qobject_cast<GeneralPluginInterface *>(plugintypeb.data());
			if (typebinterface)
			{
				QVariantHash args;
				qDebug() << "fileName=" << fileName << "generalCommunication=" << typebinterface->generalCommunication(args);
				plugintypeb.clear();
			}

			QSharedPointer<QObject> plugindefault = onefactory->createPluginObject("default");
			GeneralPluginInterface *defaultinterface = qobject_cast<GeneralPluginInterface *>(plugindefault.data());
			if (defaultinterface)
			{
				QVariantHash args;
				qDebug() << "fileName=" << fileName << "generalCommunication=" << defaultinterface->generalCommunication(args);
				plugindefault.clear();
			}
		}

		loader.unload();
	}

	return app.exec();
}
