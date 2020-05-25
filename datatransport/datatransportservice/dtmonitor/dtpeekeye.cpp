#include <QDebug>
#include <QCoreApplication>
#include <QTextCodec>

#include "dtpeekeye.h"
#include "datatransportwraper.h"
#include "datatransportconfigxml.h"

DtPeekEye::DtPeekEye(QObject* parent, const QString &filename)
{
	QString execpath = QCoreApplication::applicationDirPath();

	QString xmlfilename = execpath + "/" + filename;
	qDebug() << "xmlfilename=" << xmlfilename;
	DataTransportConfigXml directorxml(xmlfilename, "director");
	QVariantList directorlist = directorxml.getSubTagList();
	if (directorlist.size() > 0)
	{
		QVariantHash directorconfig = directorlist.at(0).toHash();
		QSharedPointer<DataTransportWraper> directordtwraper = QSharedPointer<DataTransportWraper>(new DataTransportWraper(directorconfig));
		if (directordtwraper)
		{
			bool connectifok = connect(directordtwraper.data(), SIGNAL(readData(const QList<QByteArray> &)), this, SLOT(handleDirectorData(const QList<QByteArray> &)));

			dtdirector = directordtwraper;
			qDebug() << "director name=" << directordtwraper->getName() << " desc=" << directordtwraper->getDesc() << " debug=" << directordtwraper->getDebug() << " connectifok=" << connectifok;
		}
	}
}

void DtPeekEye::handleDirectorData(const QList<QByteArray> &data)
{
	qDebug() << "enter handleDirectorData";

	foreach(const QByteArray &everyframe, data)
	{
		qDebug() << "everyframe=" << everyframe.toHex();

		int framelen = everyframe.size();
		if (framelen < 0)
			break;

		unsigned char flagbyte = (unsigned char)everyframe[0];

		if ((flagbyte & 0x01) != 0x01)
			break;

		if ((flagbyte & 0x02) != 0x00)
			break;

		if ((flagbyte & 0xFC) != 0x00)
			break;

		int nametail = everyframe.indexOf('\0', 1);
		if (nametail <= 0)
			break;

		int bytestart = nametail + 1;
		if (framelen <= bytestart)
			break;

		QString dtname = everyframe.mid(1, nametail - 1);

		QByteArray sendframe = everyframe.mid(bytestart);
	}

	qDebug() << "leave handleDirectorData";
}

