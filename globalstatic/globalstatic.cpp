#include <QCoreApplication>
#include <QDebug> 
#include <QGlobalStatic> 

#if QT_VERSION>=QT_VERSION_CHECK(5,1,0)
class UserValueType
{
	private:
		int value;

	public:
		UserValueType():value(0){};
		UserValueType(int val):value(val){};
		int getValue(){return value;};
};

Q_GLOBAL_STATIC( UserValueType, gUserValue1 )
Q_GLOBAL_STATIC_WITH_ARGS( UserValueType, gUserValue2, (100) )

bool UserFunction()
{
	qDebug() << "in UserFunction";
	return true;
}

Q_GLOBAL_STATIC_WITH_ARGS(bool, gUserFunc1, (UserFunction()))

#endif

int main(int argc, char *argv[])
{
	QCoreApplication coreApplication(argc, argv);

	/* ִ����UserFunction */
	gUserFunc1();

#if QT_VERSION>=QT_VERSION_CHECK(5,1,0)
	/* ע��������һ��,���в�ͬ����� */
	int tmpvalue1 = gUserValue1->getValue();
	UserValueType *tmpvalue2 = gUserValue2();

	if (gUserValue1.exists())
		qDebug()<<"gUserValue1.getValue="<<gUserValue1->getValue();
	if (gUserValue2.exists())
		qDebug() << "gUserValue2.getValue=" << gUserValue2->getValue();
#endif

	return coreApplication.exec();
}
