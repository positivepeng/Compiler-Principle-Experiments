#include "tree.h"
#include "project2.tab.h"

node* newNode(int tokenType, char* text){
	node* p = (node*)malloc(sizeof(node));
	p->rowNum = p->colNum = 0;
	p->val = -1;
	p->name = NULL;
	if(tokenType == ID){
		p->name = malloc(strlen(text));
		strcpy(p->name, text);
	}
	p->childs = p->next = p->par = NULL;
	return p;
}