#include <stdio.h>
#include <stdarg.h>
int averge(int num, ...) {
    // va_list valist;
    // double sum;
    // va_start(valist, num);
    // for (int i = 0; i < num; i++) {
    //     sum = sum + va_arg(valist, int);//参数2说明返回的类型为int
    // }
    // va_end(valist);
    // return sum / num;
    va_list valist;
    double sum = 0;
    va_start(valist, num);
    int k = va_arg(valist, int);
    for(int i = 1;i < num; i++)
    	sum += va_arg(valist, int);
   	va_end(valist);
   	return sum / (num-1);
}

int main(int argc, char const *argv[])
{
	printf("averge: %d\n", averge(3, 5, 10, 20));
	return 0;
}