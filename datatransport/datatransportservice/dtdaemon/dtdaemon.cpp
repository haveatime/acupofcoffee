#include <QCoreApplication>
#include <QSettings>
#include <QDir>

#include "qtservice.h"
#include "dtcoordinator.h"

class DataTransportService : public QtService<QCoreApplication>
{
public:
    DataTransportService(int argc, char **argv)
	: QtService<QCoreApplication>(argc, argv, "dtdaemon")
    {
        setServiceDescription("data transport service");
        setServiceFlags(QtServiceBase::CanBeSuspended);
    }

protected:
    void start()
    {
        QCoreApplication *app = application();

		coordinator = new DtCoordinator(app);
    }

    void pause()
    {

    }

    void resume()
    {

    }

private:
	DtCoordinator *coordinator;
};

int main(int argc, char **argv)
{
#if !defined(Q_OS_WIN)
    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
    qWarning("(settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
		/* QtSoftware.conf不能改,因为在qtservice/src下的源码中已经指定了. */
#endif
	DataTransportService service(argc, argv);
    return service.exec();
}
