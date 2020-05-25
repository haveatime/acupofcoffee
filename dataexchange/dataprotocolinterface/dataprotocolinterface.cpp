
#include <QDebug>
#include "dataprotocolinterface.h"

DataProtocolInterface::DataProtocolInterface(const QVariantHash &parameter)
{
	name = parameter["name"].toString();
	desc = parameter["desc"].toString();
	debug = parameter["debug"].toInt();
}

DataProtocolInterface::~DataProtocolInterface()
{

}

QString DataProtocolInterface::getName()
{
	return name;
}

QString DataProtocolInterface::getDesc()
{
	return desc;
}

int DataProtocolInterface::getDebug()
{
	return debug;
}
