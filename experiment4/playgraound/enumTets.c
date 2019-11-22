#include <stdio.h>

typedef enum{
	INT = 1, FLOAT
}type;


int main(int argc, char const *argv[])
{
	type a = INT;
	printf("%d\n", a);
	return 0;
}