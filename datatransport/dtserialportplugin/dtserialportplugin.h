#ifndef DT__SERIALPORT__PLUGIN__H
#define DT__SERIALPORT__PLUGIN__H

#include "datatransportinterface.h"
#include <QSerialPort>

#ifdef DT_SERIALPORT_PLUGIN_EXPORT
#define DT_SERIALPORT_PLUGIN_DECL Q_DECL_EXPORT 
#else
#define DT_SERIALPORT_PLUGIN_DECL Q_DECL_IMPORT 
#endif

class DT_SERIALPORT_PLUGIN_DECL DtSerialPortFactory: public QObject, public DataTransportFactory
{
	Q_OBJECT

	Q_INTERFACES(DataTransportFactory)
#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
	Q_PLUGIN_METADATA(IID "tools.datatransportfactory.DataTransportFactory" FILE "dtserialportfactory.json")
#endif

public:
	virtual ~DtSerialPortFactory() {}

	virtual QSharedPointer<QObject> createOneDataTransportInstance( const QVariantHash &parameter );
};

class DT_SERIALPORT_PLUGIN_DECL DtSerialPortPlugin : public DataTransportInterface
{
	Q_OBJECT

private:
	QString serialname;
	QSerialPort::BaudRate serialbaud;
	QSerialPort::DataBits serialdata;
	QSerialPort::StopBits serialstop;
	QSerialPort::Parity serialparity;
	QSerialPort serialport;

public:
	DtSerialPortPlugin( const QVariantHash &parameter );
	virtual ~DtSerialPortPlugin();

public slots:
	void processData();

public slots:
	virtual void writeData(const QList<QByteArray> &data);
};

#endif
