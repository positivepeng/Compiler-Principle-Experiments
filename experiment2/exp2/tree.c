#include "tree.h"

node* newNode(int tokenType, char* text, struct YYLTYPE* loc){
	node* p = (node*)malloc(sizeof(node));
	p->lineNum = loc->first_line;
	p->colNum = loc->first_column;
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
	else if(tokenType == SEMI){
		p->name = strdup("SEMI");	
	}
	else if(tokenType == COMMA){
		p->name = strdup("COMMA");	
	}
	else if(tokenType == ASSIGNOP){
		p->name = strdup("ASSIGNOP");	
	}
	else if(tokenType == RELOP){
		p->name = strdup("RELOP");	
	}
	else if(tokenType == PLUS){
		p->name = strdup("PLUS");	
	}
	else if(tokenType == MINUS){
		p->name = strdup("MINUS");	
	}
	    else if(tokenType == STAR){
        p->name = strdup("STAR");
	}
    else if(tokenType == DIV){
        p->name = strdup("DIV");
	}
    else if(tokenType == AND){
        p->name = strdup("AND");
	}
    else if(tokenType == OR){
        p->name = strdup("OR");
	}
    else if(tokenType == DOT){
        p->name = strdup("DOT");
	}
    else if(tokenType == NOT){
        p->name = strdup("NOT");
	}
    else if(tokenType == TYPE){
        p->name = strdup("TYPE");
	}
    else if(tokenType == LP){
        p->name = strdup("LP");
	}
    else if(tokenType == RP){
        p->name = strdup("RP");
	}
    else if(tokenType == LB){
        p->name = strdup("LB");
	}
    else if(tokenType == RB){
        p->name = strdup("RB");
	}
    else if(tokenType == LC){
        p->name = strdup("LC");
	}
    else if(tokenType == RC){
        p->name = strdup("RC");
	}
    else if(tokenType == STRUCT){
        p->name = strdup("STRUCT");
	}
    else if(tokenType == RETURN){
        p->name = strdup("RETURN");
	}
    else if(tokenType == IF){
        p->name = strdup("IF");
	}
    else if(tokenType == ELSE){
        p->name = strdup("ELSE");
	}
    else if(tokenType == WHILE){
        p->name = strdup("WHILE");
	}

	p->childs = p->next = p->par = NULL;
	return p;
}


