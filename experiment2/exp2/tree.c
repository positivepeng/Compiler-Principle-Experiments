#include "tree.h"

node* newNode(int tokenType, char* text){
	node* p = (node*)malloc(sizeof(node));
	p->rowNum = p->colNum = 0;
	p->val.ival = -1;
	p->name = NULL;

	if(tokenType == ID){
		p->name = malloc(strlen(text));
		strcpy(p->name, text);
	}
	else if(tokenType == INT){
		p->name = strdup("INT");
		p->val.ival = atoi(text);
	}
	else if(tokenType == FLOAT){
		p->name = strdup("FLOAT");
		p->val.fval = atof(text);	
	}

	p->childs = p->next = p->par = NULL;

	return p;
}