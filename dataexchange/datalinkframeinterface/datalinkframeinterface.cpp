
#include <QDebug>
#include "datalinkframeinterface.h"

DataLinkframeInterface::DataLinkframeInterface(const QVariantHash &parameter)
{
	name = parameter["name"].toString();
	desc = parameter["desc"].toString();
	debug = parameter["debug"].toInt();
}

DataLinkframeInterface::~DataLinkframeInterface()
{

}

QString DataLinkframeInterface::getName()
{
	return name;
}

QString DataLinkframeInterface::getDesc()
{
	return desc;
}

int DataLinkframeInterface::getDebug()
{
	return debug;
}
