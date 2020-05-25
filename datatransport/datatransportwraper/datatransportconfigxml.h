#ifndef DATA__TRANSPORT__CONFIG__XML__H
#define DATA__TRANSPORT__CONFIG__XML__H

#include <QString>
#include <QVariant>

class QDomElement;

#ifdef DATA_TRANSPORT_WRAPER_EXPORT
#define DATA_TRANSPORT_WRAPER_DECL Q_DECL_EXPORT 
#else
#define DATA_TRANSPORT_WRAPER_DECL Q_DECL_IMPORT 
#endif

class DATA_TRANSPORT_WRAPER_DECL DataTransportConfigXml
{
private:
	QString cfgfilename;
	QString subtagname;
	QVariantList subtaglist;

	QVariantHash parseSubTagConfig(const QString& subtagattr, const QDomElement& subelement);
	void readXmlConfig();

public:
	DataTransportConfigXml(const QString &filename, const QString &tagname);

	QVariantList getSubTagList();
};

#endif
