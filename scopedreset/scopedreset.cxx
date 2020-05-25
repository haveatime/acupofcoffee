#include <QDebug>
#include <QCoreApplication>

#include <QScopedValueRollback>

int main(int argc, char **argv)
{
	QCoreApplication qtapp(argc, argv);

	int oneint = 100;
	{
		QScopedValueRollback<int>  tempset(oneint);
		oneint = 300;
		qDebug() << "inscope oneint=" << oneint;
	}
	qDebug() << "outscope oneint=" << oneint;

	bool onebool = false;
	{
		QScopedValueRollback<bool>  tempset(onebool);
		onebool = true;
		qDebug() << "inscope onebool=" << onebool;
	}
	qDebug() << "outscope onebool=" << onebool;

	double onedouble = 1.1;
	{
		QScopedValueRollback<double>  tempset(onedouble);
		onedouble = 2.2;
		tempset.commit();
		qDebug() << "outscope onedouble=" << onedouble;
	}
	qDebug() << "outscope onedouble=" << onedouble;

	qtapp.exec();

	return 0;
}

