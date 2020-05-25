
#include <QDebug>
#include "dpiec104plugin.h"

QSharedPointer<QObject> DpIec104Factory::createOneDataProtocolInstance(const QVariantHash &parameter)
{
	return QSharedPointer<QObject>(new DpIec104Plugin(parameter));
}

#if QT_VERSION<QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(staticdpiec104factory, DpIec104Factory)
#endif

DpIec104Plugin::DpIec104Plugin(const QVariantHash &parameter)
:DataProtocolInterface(parameter)
{
}

DpIec104Plugin::~DpIec104Plugin()
{
}

void DpIec104Plugin::feedInds(const QList<QJsonObject> &data)
{
}

void DpIec104Plugin::feedBytes(const QList<QByteArray> &data)
{
}
