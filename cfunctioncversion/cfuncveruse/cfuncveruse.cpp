#include <QObject>

#include <stdio.h>

/******************************************************************
使用symver管理函数版本
还以可以type指定gnu_indirect_function使用间接函数
******************************************************************/

extern "C"
{
/******************************************************************
注释下一行会使用testfuncion的默认版本,否则使用注释中的指定版本
******************************************************************/
__asm__(".symver testfuncion,testfuncion@VER1.0");

int Q_DECL_IMPORT testfuncion();
}

int main(int argc, char **argv)
{
	printf("\ntestfuncion=%d\n",testfuncion());
}
