#include <QCoreApplication>
#include <QNetworkInterface> 
#include <QHostInfo>
#include <QDebug> 

void ListAllNetworkInterfaces()
{
	QList<QNetworkInterface> allnetcard = QNetworkInterface::allInterfaces();

	for (QList<QNetworkInterface>::iterator cardindex = allnetcard.begin(); cardindex != allnetcard.end(); cardindex++)
	{
		qDebug() << "index=" << cardindex->index() << cardindex->flags() << cardindex->hardwareAddress() << cardindex->name();

		QList<QNetworkAddressEntry> allentry = cardindex->addressEntries();
		for (QList<QNetworkAddressEntry>::iterator entryindex = allentry.begin(); entryindex != allentry.end(); entryindex++)
		{
			qDebug() << "    entry=" << entryindex->ip().toString() << entryindex->netmask().toString() << entryindex->broadcast().toString();
		}
	}
}

void ListAllAddressesOfLocalName()
{
	QString localname = QHostInfo::localHostName();
	qDebug() << "hostname=" << localname;
	QHostInfo localhost = QHostInfo::fromName(localname);

	QList<QHostAddress> alladdresses = localhost.addresses();
	for (QList<QHostAddress>::iterator addressindex = alladdresses.begin(); addressindex != alladdresses.end(); addressindex++)
	{
		qDebug() << "address=" << addressindex->toString() << addressindex->protocol();
	}
}

int main(int argc, char *argv[])
{
	QCoreApplication coreApplication(argc, argv);

	qDebug() << endl << "**********ListAllNetworkInterfaces**********";
	ListAllNetworkInterfaces();


	qDebug() << endl << "**********ListAllAddressesOfLocalName**********";
	ListAllAddressesOfLocalName();

	return coreApplication.exec();
}
