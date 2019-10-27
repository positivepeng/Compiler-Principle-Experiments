#include "tree.h"

node* newNode(){
	node* p = (node*)malloc(sizeof(node));
	p->rowNum = p->colNum = 0;
	p->val = -1;
	p->name = NULL;
	p->childs = p->next = p->par = NULL;
	return p;
}

void test(){
	node* p = newNode();
	printf("init node\n");
	printf("init val:%d\n", p->val);
	printf("init rowNum:%d colNum: %d\n", p->rowNum, p->colNum);
	if(p->childs == NULL)
	    printf("No child\n");
}

int main(int argc, char const *argv[])
{
	// test();
	return 0;
}