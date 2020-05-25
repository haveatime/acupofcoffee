#include <stdio.h>

int main( int argc, char **argv )
{
#ifdef IS_SOME_THING_YES
	printf("macro IS_SOME_THING_YES is defined!\n");
#else
	printf("macro IS_SOME_THING_YES isnt defined!\n");
#endif

	return 0;
}

