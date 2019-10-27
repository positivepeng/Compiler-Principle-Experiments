#include <stdio.h>
struct node1
{
	int left, right;
	struct node1* child, *next;
};
typedef struct node1 node;

void test(node a){
	a.left = 23;
	a.right = 23;
	printf("%d\n", a.left);
}

int main(int argc, char const *argv[])
{
	node a;
	test(a);
	return 0;
}