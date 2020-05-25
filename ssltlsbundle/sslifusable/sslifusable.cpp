#include <QCoreApplication>
#include <QDebug>
#include <QSslSocket>
#include <QSslConfiguration>
#include <QSslCipher>
#include <QSslEllipticCurve>

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

#ifndef	QT_NO_SSL
	qDebug() << "sslLibraryBuildVersionNumber=" << QSslSocket::sslLibraryBuildVersionNumber();
	qDebug() << "sslLibraryVersionNumber=" << QSslSocket::sslLibraryVersionNumber();

	qDebug() << "sslLibraryBuildVersionString=" << QSslSocket::sslLibraryBuildVersionString();
	qDebug() << "sslLibraryVersionString=" << QSslSocket::sslLibraryVersionString();

	qDebug() << "supportsSsl=" << QSslSocket::supportsSsl();

	foreach(const QSslCipher &everycipher, QSslConfiguration::supportedCiphers())
		qDebug() << "supportedCiphers=" << everycipher.name();
	qDebug() << endl;

	foreach(const QSslEllipticCurve &everycurves, QSslConfiguration::supportedEllipticCurves())
		qDebug() << "supportedEllipticCurves=" << everycurves.longName();
	qDebug() << endl;

#else
	qDebug() << "Qt configured no ssl.";
#endif

	return app.exec();
}
