#include "tree.h"

node* newNode(int tokenType, char* text){
	node* p = (node*)malloc(sizeof(node));
	p->rowNum = p->colNum = 0;
	p->val = -1;
	p->name = NULL;
	if(tokenType == ID){
		p->name = (char*)malloc(sizeof(node));
		strcpy(p->name, text);
	}
	p->childs = p->next = p->par = NULL;
	return p;
}