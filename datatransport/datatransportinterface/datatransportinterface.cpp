
#include <QDebug>
#include "datatransportinterface.h"

DataTransportInterface::DataTransportInterface(const QVariantHash &parameter)
{
	name = parameter["name"].toString();
	desc = parameter["desc"].toString();
	debug = parameter["debug"].toInt();
}

DataTransportInterface::~DataTransportInterface()
{

}

QString DataTransportInterface::getName()
{
	return name;
}

QString DataTransportInterface::getDesc()
{
	return desc;
}

int DataTransportInterface::getDebug()
{
	return debug;
}
