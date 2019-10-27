#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char const *argv[])
{
	char* c = strdup("Hello");
	printf("%s\n", c);
	return 0;
}