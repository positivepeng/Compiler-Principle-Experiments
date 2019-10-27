#include <stdio.h>
#include <stdlib.h>

struct node{
	int val;
	char* name;
	struct node* childs, *next, *par;
};
typedef struct node node;

void initNodePointer(node* np){
	np->val = -1;
	np->name = NULL;
	np->childs = np->next = np->par = NULL;
}

int main(int argc, char const *argv[])
{
	node* np = (node*)malloc(sizeof(node));
	initNodePointer(np);

	printf("%d\n", np->val);
	if(np->name == NULL)
		printf("NULL name\n");
	if(np->childs == NULL)
		printf("NO childs\n");

	free(np);
	return 0;
}
