#include <QString>
#include <QTextCodec>
#include <QMutex>
#include <QDateTime>
#include <QFile>
#include <QProcessEnvironment> 
#include <QTextStream>
#include <QDir>

#define LOG_MAX 100*1024*1024
static QMutex log_mutex;

#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
void messageLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
#else
void messageLogger(QtMsgType type, const char *msg)
#endif
{
	QTextCodec *gb18030codec = QTextCodec::codecForName("GB18030");
	QString txt;
	QMutexLocker locker(&log_mutex);

#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
	QString qmsg = msg;
#else
	QString qmsg = gb18030codec->toUnicode(msg);
#endif
	switch (type)
	{
	case QtDebugMsg:
		txt = QString::fromLocal8Bit("调试信息: %1").arg(qmsg);
		break;

	case QtWarningMsg:
		txt = QString::fromLocal8Bit("警告信息: %1").arg(qmsg);
		break;

	case QtCriticalMsg:
		txt = QString::fromLocal8Bit("严重错误: %1").arg(qmsg);
		break;

	case QtFatalMsg:
		txt = QString::fromLocal8Bit("致命错误: %1").arg(qmsg);
		break;

	default:
		txt = QString::fromLocal8Bit("其它信息: %1").arg(qmsg);
		break;
	}

	QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

	QString log_str = QProcessEnvironment::systemEnvironment().value("LOG_DIR", ".");
	QDir logDir(log_str);

	QString filename = logDir.absoluteFilePath("messagelogger.log");
	QString bakfilename = logDir.absoluteFilePath("messagelogger.bak.log");
	QFile outFile(filename);

	if (outFile.size() >= LOG_MAX)
	{
		QFile(bakfilename).remove();
		outFile.copy(bakfilename);
		outFile.resize(0);
	}

	if (!outFile.open(QIODevice::Append | QIODevice::Text))
		return;

	QTextStream outstream(&outFile);
	outstream.setCodec(gb18030codec);
	outstream << current_date << " "<< txt << endl;
}
