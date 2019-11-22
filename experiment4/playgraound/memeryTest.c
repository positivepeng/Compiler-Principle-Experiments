#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

int main(int argc, char const *argv[])
{
	char * name = strdup("hello");
	printf("%s\n", name);
	free(name);
	return 0;
}