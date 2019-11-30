#include <stdio.h>
union val{
	int ival;
	float fval;
};
int main(int argc, char const *argv[])
{
	union val s;
	s.fval = 23.4;
	printf("%lf\n", s.fval);
	s.ival = 10;
	printf("%d\n", s.ival);
	return 0;
}