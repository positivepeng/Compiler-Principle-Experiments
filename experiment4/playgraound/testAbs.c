#include <stdio.h>
#include <math.h>
int main()
{
    int a = 3, b = -4, c, d;  //为变量赋初值
    c = abs(a);  //求a的绝对值
    d = abs(b);  //求b的绝对值
    printf("%d\n%d\n", c, d);
    return 0;
}