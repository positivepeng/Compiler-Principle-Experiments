#include <stdio.h>
void change(int* a, int* index ,int num){
	a[*index] = num;
	(*index)++;
}
int main(int argc, char const *argv[])
{
	int a[5] = {0,0,0,0,0};
	int k = 3;
	change(a, &k, 23);
	for(int i = 0;i < 5; i++)
		printf("%d ", a[i]);
	printf("\n%d", k);
	return 0;
}