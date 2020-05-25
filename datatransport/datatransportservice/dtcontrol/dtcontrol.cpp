#include <stdio.h>

#include <QCoreApplication>
#include <QSettings>
#include <QDir>

#include "qtservice.h"

int processArgs(int argc, char **argv)
{
    if (argc > 2) {
        QString arg1(argv[1]);
	if (arg1 == QLatin1String("-i") ||
            arg1 == QLatin1String("-install")) {
	    if (argc > 2) {
                QString account;
                QString password;
		QString path(argv[2]);
                if (argc > 3)
                    account = argv[3];
                if (argc > 4)
                    password = argv[4];
                printf("The service %s installed.\n",
                       (QtServiceController::install(path, account, password) ? "was" : "was not"));
                return 0;
            }
        } else {
            QString serviceName(argv[1]);
            QtServiceController controller(serviceName);
            QString option(argv[2]);
            if (option == QLatin1String("-u") ||
                option == QLatin1String("-uninstall")) {
                printf("The service \"%s\" %s uninstalled.\n",
                            controller.serviceName().toLatin1().constData(),
                            (controller.uninstall() ? "was" : "was not"));
                return 0;
            } else if (option == QLatin1String("-s") ||
                       option == QLatin1String("-start")) {
                QStringList args;
                for (int i = 3; i < argc; ++i)
                    args.append(QString::fromLocal8Bit(argv[i]));
                printf("The service \"%s\" %s started.\n",
                       controller.serviceName().toLatin1().constData(),
                            (controller.start(args) ? "was" : "was not"));
                return 0;
            } else if (option == QLatin1String("-t") ||
                       option == QLatin1String("-terminate")) {
                printf("The service \"%s\" %s stopped.\n",
                       controller.serviceName().toLatin1().constData(),
                       (controller.stop() ? "was" : "was not"));
                return 0;
            } else if (option == QLatin1String("-v") ||
                    option == QLatin1String("-version")) {
                bool installed = controller.isInstalled();
                printf("The service\n"
                        "\t\"%s\"\n\n", controller.serviceName().toLatin1().constData());
                printf("is %s", (installed ? "installed" : "not installed"));
                printf(" and %s\n\n", (controller.isRunning() ? "running" : "not running"));
                if (installed) {
                    printf("path: %s\n", controller.serviceFilePath().toLatin1().data());
                    printf("description: %s\n", controller.serviceDescription().toLatin1().data());
                    printf("startup: %s\n", controller.startupType() == QtServiceController::AutoStartup ? "Auto" : "Manual");
                }
                return 0;
            }
        }
    }
    printf("dtcontrol [-i PATH | SERVICE_NAME [-v | -u | -s | -t | -h]\n\n"
            "\t-i(nstall) PATH\t: Install the service\n"
            "\t-v(ersion)\t: Print status of the service\n"
            "\t-u(ninstall)\t: Uninstall the service\n"
            "\t-s(tart)\t: Start the service\n"
            "\t-t(erminate)\t: Stop the service\n"
            "\t-h(elp)\t\t: Print this help info\n");
    return 0;
}

int main(int argc, char **argv)
{
#if !defined(Q_OS_WIN)
    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
    qWarning("(settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
		/* QtSoftware.conf不能改,因为在qtservice/src下的源码中已经指定了. */
#endif

    int result = processArgs(argc, argv);

    return result;
}
