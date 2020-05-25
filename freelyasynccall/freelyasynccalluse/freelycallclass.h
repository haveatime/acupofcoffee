#ifndef FREELY__CALL__CLASS__H
#define FREELY__CALL__CLASS__H

#include <QObject>
#include <QVariant>

class FreelyCallClass : public QObject
{
	Q_OBJECT

public slots:
	void FreelyCallSlot(const QVariantHash &data);

};

#endif
