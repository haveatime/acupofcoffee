#include <QObject>

extern "C"
{
__asm__(".symver ver1_testfuncion,testfuncion@VER1.0");
__asm__(".symver ver2_testfuncion,testfuncion@@VER2.0");

int Q_DECL_EXPORT ver1_testfuncion();
int Q_DECL_EXPORT ver1_testfuncion();

int ver1_testfuncion()
{
	return 1;
}

int ver2_testfuncion()
{
	return 2;
}

}
