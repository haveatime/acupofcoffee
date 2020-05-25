#include <QDebug>
#include <QCoreApplication>
#include <QTextCodec>

#include "dtcoordinator.h"
#include "datatransportwraper.h"
#include "datatransportconfigxml.h"

DtCoordinator::DtCoordinator(QObject* parent)
	:QObject(parent), debug(0)
{
	QString execpath = QCoreApplication::applicationDirPath();
	QString appname = QCoreApplication::applicationName();

	QString xmlfilename = execpath + "/" + appname + ".xml";
	qDebug() << "xmlfilename=" << xmlfilename;
	DataTransportConfigXml directorxml(xmlfilename, "director");
	QVariantList directorlist = directorxml.getSubTagList();
	if (directorlist.size() > 0)
	{
		QVariantHash directorconfig = directorlist.at(0).toHash();
		QSharedPointer<DataTransportWraper> directordtwraper = QSharedPointer<DataTransportWraper>(new DataTransportWraper(directorconfig));
		if (directordtwraper)
		{
			debug = directordtwraper->getDebug();
			bool connectifok = connect(directordtwraper.data(), SIGNAL(readData(const QList<QByteArray> &)), this, SLOT(handleDirectorData(const QList<QByteArray> &)));

			dtdirector = directordtwraper;
			qDebug() << "director name=" << directordtwraper->getName() << " desc=" << directordtwraper->getDesc() << " debug=" << directordtwraper->getDebug() << " connectifok=" << connectifok;
		}
	}

	DataTransportConfigXml transportxml(xmlfilename, "transport");
	QVariantList transportlist = transportxml.getSubTagList();

	int transportcount = transportlist.count();
	for (int index = 0; index < transportcount; index++)
	{
		QVariantHash transportconfig = transportlist.at(index).toHash();
		QString transportname = transportconfig["name"].toString();
		QSharedPointer<DataTransportWraper> transportdtwraper = QSharedPointer<DataTransportWraper>(new DataTransportWraper(transportconfig));
		if (transportdtwraper)
		{
			bool connectifok = connect(transportdtwraper.data(), SIGNAL(readData(const QList<QByteArray> &)), this, SLOT(handleTransportData(const QList<QByteArray> &)));

			dttransport[transportname] = transportdtwraper;
			qDebug() << "transport name=" << transportdtwraper->getName() << " desc=" << transportdtwraper->getDesc() << " connectifok=" << connectifok;
		}
	}
}

void DtCoordinator::handleTransportData(const QList<QByteArray> &data)
{
	if (debug != 0)
		qDebug() << "enter handleTransportData";

	QObject *whosend = sender();
	DataTransportWraper *dtchildwraper = qobject_cast<DataTransportWraper *>(whosend);
	if (debug != 0)
		qDebug() << "dtchildwraper=" << dtchildwraper;
	if (!dtchildwraper)
		return;

	unsigned char flagbit = 0;
	QString dtname = dtchildwraper->getName();

	QByteArray framehead;
	framehead.append((char)flagbit);
	framehead.append(dtname);
	framehead.append('\0');

	DataTransportWraper *directorwraper = qobject_cast<DataTransportWraper *>(dtdirector.data());
	if (debug != 0)
		qDebug() << "directorwraper=" << directorwraper;
	if (!directorwraper)
		return;

	foreach(const QByteArray &everyframe, data)
	{
		QByteArray extendframe = framehead;
		extendframe.append(everyframe);
		if (debug != 0)
			qDebug() << "recvframe=" << everyframe.toHex();
		QList<QByteArray> framepacket;
		framepacket.append(extendframe);
		directorwraper->writeData(framepacket);
	}

	if (debug != 0)
		qDebug() << "leave handleTransportData";
}

void DtCoordinator::handleDirectorData(const QList<QByteArray> &data)
{
	if (debug != 0)
		qDebug() << "enter handleDirectorData";

	foreach(const QByteArray &everyframe, data)
	{
		int framelen = everyframe.size();
		if (debug != 0)
			qDebug() << "framelen=" << framelen;

		if (framelen < 0)
			break;

		unsigned char flagbyte = (unsigned char)everyframe[0];
		if (debug != 0)
			qDebug() << "flagbit=" << (int)flagbyte;

		if ((flagbyte & 0x01) != 0x01)
			break;

		if ((flagbyte & 0x02) != 0x00)
			break;

		if ((flagbyte & 0xFC) != 0x00)
			break;

		int nametail = everyframe.indexOf('\0', 1);
		if (debug != 0)
			qDebug() << "nametail=" << nametail;
		if (nametail <= 0)
			break;

		int bytestart = nametail + 1;
		if (debug != 0)
			qDebug() << "bytestart=" << bytestart;
		if (framelen <= bytestart)
			break;

		QString dtname = everyframe.mid(1, nametail - 1);
		if (debug != 0)
			qDebug() << "dtname=" << dtname << " bytestart=" << bytestart;
		if (!dttransport.contains(dtname))
			break;

		QSharedPointer<QObject> subtransport = dttransport[dtname];
		DataTransportWraper *dtchildwraper = qobject_cast<DataTransportWraper *>(subtransport.data());
		if (debug != 0)
			qDebug() << "dtchildwraper=" << dtchildwraper;
		if (!dtchildwraper)
			break;

		QByteArray sendframe = everyframe.mid(bytestart);
		if (debug != 0)
			qDebug() << "sendframe=" << sendframe.toHex();
		QList<QByteArray> framepacket;
		framepacket.append(sendframe);
		dtchildwraper->writeData(framepacket);
	}

	if (debug != 0)
		qDebug() << "leave handleDirectorData";
}
