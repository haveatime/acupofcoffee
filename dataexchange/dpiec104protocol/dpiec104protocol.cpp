
#include <QDebug>
#include "dpiec104protocol.h"
#include "dliec104frame.h"

QSharedPointer<QObject> DpIec104Factory::createOneDataProtocolInstance(const QVariantHash &parameter)
{
	return QSharedPointer<QObject>(new DpIec104Protocol(parameter));
}

#if QT_VERSION<QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(staticdpiec104factory, DpIec104Factory)
#endif

DpIec104Protocol::DpIec104Protocol(const QVariantHash &parameter)
:DataProtocolInterface(parameter)
{
	parseIec104Frame = QSharedPointer<DataLinkframeInterface>(new DlIec104Frame(parameter));
}

DpIec104Protocol::~DpIec104Protocol()
{
}

void DpIec104Protocol::feedInds(const QList<QJsonObject> &data)
{
}

void DpIec104Protocol::feedBytes(const QList<QByteArray> &data)
{
}
