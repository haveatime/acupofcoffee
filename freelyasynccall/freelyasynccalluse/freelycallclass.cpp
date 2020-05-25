#include "freelycallclass.h"

#include <QDebug>

void FreelyCallClass::FreelyCallSlot(const QVariantHash &data)
{
	QObject *whosend = sender();
	qDebug() << "FreelyCallClass::FreelyCallSlot" << "whosend=" << whosend << "data=" << data;
}
