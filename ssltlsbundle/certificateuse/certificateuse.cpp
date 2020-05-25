#include <QCoreApplication>
#include <QDebug>
#include <QSslSocket>
#include <QDir>
#include <QFile>
#include <QSslCertificate>
#include <QSslKey>

void displaykey(const QSslKey & sslkey)
{
	qDebug() << "********** displaykey **********";

	if (sslkey.isNull())
		qDebug() << "isNull";
	else
	{
		qDebug() << "\t" << "key=" << sslkey;
		qDebug() << "\t" << "type=" << sslkey.type();
		qDebug() << "\t" << "algorithm=" << sslkey.algorithm();
		qDebug() << "\t" << "toPem=" << sslkey.toPem();
	}

	qDebug() << "****************************************";
}

void displaycertificate( const QSslCertificate & sslcert )
{
	qDebug() << "********** displaycertificate **********";

	if (sslcert.isNull())
		qDebug() << "isNull";
	else
	{
		qDebug() << "subjectInfo:";
		foreach(const QByteArray &everysubjectattribute, sslcert.subjectInfoAttributes())
		{
			qDebug() << "\t" << everysubjectattribute << "=" << sslcert.subjectInfo(everysubjectattribute);
		}

		qDebug() << "issuerInfo:";
		foreach(const QByteArray &everyissuerattribute, sslcert.issuerInfoAttributes())
		{
			qDebug() << "\t" << everyissuerattribute << "=" << sslcert.issuerInfo(everyissuerattribute);
		}

		qDebug() << "effectiveDate:";
		qDebug() << "\t" << sslcert.effectiveDate();

		qDebug() << "expiryDate=:";
		qDebug() << "\t" << sslcert.expiryDate();

		qDebug() << "serialNumber=:";
		qDebug() << "\t" << sslcert.serialNumber();

		qDebug() << "version=:";
		qDebug() << "\t" << sslcert.version();

		QSslKey sslkey = sslcert.publicKey();
		displaykey(sslkey);

		qDebug() << "digest:";
		qDebug() << "\t" << sslcert.digest().toHex();

		qDebug() << "toPem:";
		qDebug() << "\t" << sslcert.toPem();
	}

	qDebug() << "****************************************";
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

#ifndef	QT_NO_SSL
	bool openifok = false;
	QString currentpath = QDir::currentPath();

	QString certfilename(currentpath + "/ssltlscert.cer");
	qDebug() << "certfilename=" << certfilename;
	QFile certfile(certfilename);
	openifok = certfile.open(QIODevice::ReadOnly);
	if (openifok)
	{
		QSslCertificate sslcert(certfile.readAll(),QSsl::Der);	
		displaycertificate(sslcert);

		certfile.close();
	}
	qDebug() << endl;

	QString keyfilename(currentpath + "/ssltlskey.pem");
	qDebug() << "keyfilename=" << keyfilename;
	QFile keyfile(keyfilename);
	openifok = keyfile.open(QIODevice::ReadOnly);
	if (openifok)
	{
		QSslKey sslkey( keyfile.readAll(), QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey);
		displaykey(sslkey);

		keyfile.close();
	}
	qDebug() << endl;

	QString cafilename(currentpath + "/cacert.pem");
	qDebug() << "cafilename=" << cafilename;
	QFile cafile(cafilename);
	openifok = cafile.open(QIODevice::ReadOnly);
	if (openifok)
	{
		QSslCertificate sslca(cafile.readAll(), QSsl::Pem);
		displaycertificate(sslca);

		certfile.close();
	}
	qDebug() << endl;
#endif

	return app.exec();
}
