
#include <QDebug>
#include "dtserialportplugin.h"

QSharedPointer<QObject> DtSerialPortFactory::createOneDataTransportInstance( const QVariantHash &parameter )
{
	return QSharedPointer<QObject>(new DtSerialPortPlugin(parameter));
}

#if QT_VERSION<QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(staticdtserialportfactory, DtSerialPortFactory)
#endif

DtSerialPortPlugin::DtSerialPortPlugin( const QVariantHash &parameter )
:DataTransportInterface(parameter)
{
	if (getDebug() > 0)
		qDebug() << "DtSerialPortPlugin::DtSerialPortPlugin parameter=" << parameter;

	serialname = parameter["portname"].toString();

	int baudrate = parameter["baudrate"].toInt();
	serialbaud = QSerialPort::Baud9600;
	if (baudrate == 115200)
		serialbaud = QSerialPort::Baud115200;
	else if (baudrate == 57600)
		serialbaud = QSerialPort::Baud57600;
	else if (baudrate == 38400)
		serialbaud = QSerialPort::Baud38400;
	else if (baudrate == 19200)
		serialbaud = QSerialPort::Baud19200;
	else if (baudrate == 9600)
		serialbaud = QSerialPort::Baud9600;
	else if (baudrate == 4800)
		serialbaud = QSerialPort::Baud4800;
	else if (baudrate == 2400)
		serialbaud = QSerialPort::Baud2400;
	else if (baudrate == 1200)
		serialbaud = QSerialPort::Baud1200;

	int databits = parameter["databits"].toInt();
	serialdata = QSerialPort::Data8;
	if (databits == 5)
		serialdata = QSerialPort::Data5;
	else if (databits == 6)
		serialdata = QSerialPort::Data6;
	else if (databits == 7)
		serialdata = QSerialPort::Data7;
	else if (databits == 8)
		serialdata = QSerialPort::Data8;

	QString stopbits = parameter["stopbits"].toString();
	serialstop = QSerialPort::OneStop;
	if (stopbits == "1")
		serialstop = QSerialPort::OneStop;
	else if (stopbits == "1.5")
		serialstop = QSerialPort::OneAndHalfStop;
	else if (stopbits == "2")
		serialstop = QSerialPort::TwoStop;

	QString parity = parameter["patiry"].toString();
	serialparity = QSerialPort::NoParity;
	if (parity == "no")
		serialparity = QSerialPort::NoParity;
	else if (parity == "odd")
		serialparity = QSerialPort::OddParity;
	else if (parity == "even")
		serialparity = QSerialPort::EvenParity;

	serialport.setPortName(serialname);
	serialport.setBaudRate(serialbaud);
	serialport.setDataBits(serialdata);
	serialport.setStopBits(serialstop);
	serialport.setParity(serialparity);

	connect(&serialport, SIGNAL(readyRead()), SLOT(processData()));

	bool openifok = serialport.open(QIODevice::ReadWrite);
	if (!openifok)
	{
		qDebug() << "serialname=" << serialname << "open error=" << serialport.errorString();
		return;
	}
}

DtSerialPortPlugin::~DtSerialPortPlugin()
{
	serialport.close();
}

void DtSerialPortPlugin::processData()
{
	QList<QByteArray> recvlist;

	QByteArray data = serialport.readAll();
	recvlist.append(data);

	if( recvlist.size() > 0 )
		emit readData(recvlist);
}

void DtSerialPortPlugin::writeData(const QList<QByteArray> &data)
{
	int count = data.size();

	for( int index=0; index<count; index++ )
	{
		const QByteArray &frame = data.at(index);
		serialport.write(frame);
	}
}
