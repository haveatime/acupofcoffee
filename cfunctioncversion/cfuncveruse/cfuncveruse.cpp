#include <QObject>

#include <stdio.h>

/******************************************************************
ʹ��symver�������汾
���Կ���typeָ��gnu_indirect_functionʹ�ü�Ӻ���
******************************************************************/

extern "C"
{
/******************************************************************
ע����һ�л�ʹ��testfuncion��Ĭ�ϰ汾,����ʹ��ע���е�ָ���汾
******************************************************************/
__asm__(".symver testfuncion,testfuncion@VER1.0");

int Q_DECL_IMPORT testfuncion();
}

int main(int argc, char **argv)
{
	printf("\ntestfuncion=%d\n",testfuncion());
}
