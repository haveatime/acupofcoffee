#include <QDebug>
#include <QDir>
#include <QDomElement>

#include "datatransportconfigxml.h"

DataTransportConfigXml::DataTransportConfigXml(const QString &filename, const QString &tagname)
	:cfgfilename(filename),subtagname(tagname)
{
	readXmlConfig();
}

void DataTransportConfigXml::readXmlConfig()
{
	QDomDocument doc("mydocument");
	QFile cfgfile(cfgfilename);
	if (!cfgfile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << cfgfilename << "isn't exist!";
		return;
	}

	if (!doc.setContent(&cfgfile))
	{
		cfgfile.close();
		return;
	}
	cfgfile.close();

	QDomElement docElem = doc.documentElement();

	QDomNode subnode = docElem.firstChild();
	while (!subnode.isNull())
	{
		QDomElement nodeelement = subnode.toElement();
		if (!nodeelement.isNull() && subtagname == nodeelement.tagName())
		{
			QString subtagattr = nodeelement.attribute("name");
			if (!subtagattr.isEmpty())
			{
				QVariantHash pluginhash = parseSubTagConfig(subtagattr, nodeelement);
				subtaglist.append(pluginhash);
			}
		}
		subnode = subnode.nextSibling();
	}
}

QVariantHash DataTransportConfigXml::parseSubTagConfig(const QString& subtagattr, const QDomElement& subelement)
{
	QVariantHash subtaghash;
	subtaghash["name"] = subtagattr;
	subtaghash["desc"] = subelement.attribute("desc");

	QDomNode pluginparam = subelement.firstChild();
	while (!pluginparam.isNull())
	{
		QDomElement paramelement = pluginparam.toElement();
		if (!paramelement.isNull())
		{
			QString paramtagname = paramelement.tagName();
			subtaghash[paramtagname] = paramelement.text();
		}
		pluginparam = pluginparam.nextSibling();
	}

	return subtaghash;
}

QVariantList DataTransportConfigXml::getSubTagList()
{
	return subtaglist;
}
