#include <stdio.h>

#define MACRO2STR(arg) MACRO2STR_1(arg)
#define MACRO2STR_1(arg) #arg

int main( int argc, char ** argv)
{

#define MYMAX(a,b) if(a>b) return a

	printf("\nMYMAX(1,0)=%s\n",MACRO2STR(MYMAX(1,0)));

	return 0;
}

