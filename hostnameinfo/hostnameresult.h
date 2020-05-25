#ifndef HOSTNAME__RESULT__H
#define HOSTNAME__RESULT__H

#include <QObject>
#include <QHostInfo>

class HostnameResult : public QObject
{
    Q_OBJECT

public slots:
    void lookedResult(const QHostInfo &host);

};

#endif
