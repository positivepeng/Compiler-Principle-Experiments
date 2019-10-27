#include "tree.h"

node* newNode(int tokenType, char* text, int line, int col){
	node* p = (node*)malloc(sizeof(node));
	p->lineNum = line;
	p->colNum = col;
	p->val.ival = -1;
	p->name = NULL;

	if(tokenType == ID){
		p->name = strdup("ID");
		p->val.sval = (char*)malloc(sizeof(text));
		strcpy(p->val.sval, text);
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