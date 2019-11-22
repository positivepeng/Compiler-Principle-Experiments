#include <stdio.h>

union VAL{
	int ival;
	float fval;
};

int main(int argc, char const *argv[])
{
	union VAL v1;
	v1.ival = 12;
	union VAL v2 = v1;
	printf("%d\n", v2.ival);
	return 0;
}
