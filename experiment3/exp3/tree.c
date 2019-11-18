#include "tree.h"

void printNode(int numOfTab, node* p){
	// printf("%d %s\n", numOfTab, p->name);
	if(p->tokenType == EMPTYNODETOKEN)
		return ;

	while(numOfTab--)
		printf("  ");
	if(p->tokenType == ID)
		printf("%s:%s\n", p->name, p->val.sval);
	else if(p->tokenType == INT)
		printf("%s:%i\n", p->name, p->val.ival);
	else if(p->tokenType == FLOAT)
		printf("%s:%f\n", p->name, p->val.fval);
	else if(p->tokenType == TYPE)
		printf("%s:%s\n", p->name, p->val.sval);
	else if(p->tokenType < 0)
		printf("%s (%d)\n", p->name, p->lineBegin);
	else
		printf("%s\n", p->name);
}
void dfsTraverse(int numOfTab, node* root){
	// printf("%d %s\n", numOfTab, root->name);
	printNode(numOfTab, root);

	if(root->childs != NULL){
		// dfsTraverse(numOfTab+1, root->childs);
		node* sibling = root->childs;
		while(sibling != NULL){
			dfsTraverse(numOfTab+1, sibling);
			sibling = sibling->next;
		}		
	}	
}

void addChild(int numOfChild, ...){
	va_list valist;
    va_start(valist, numOfChild);
    node* root = va_arg(valist, node*);
    node* child1 = va_arg(valist, node*);

    root->childs = child1;
	
	
	root->lineBegin = child1->lineBegin;
	root->lineEnd = child1->lineEnd;

	for(int i = 2;i < numOfChild; i++){
    	node* temp = va_arg(valist, node*);
    	if(temp != NULL){
    		root->lineEnd = temp->lineEnd;
    	}
    	child1->next = temp;
    	child1 = temp;
    }
   	va_end(valist);
   	return ;
}



node* newNode(int tokenType, char* text, struct YYLTYPE* loc){
	node* p = (node*)malloc(sizeof(node));
	
	if(loc != NULL){
		p->lineBegin = p->lineEnd = loc->first_line;
	}
	else{
		p->lineBegin = p->lineEnd = -1;
	}
	
	p->tokenType = tokenType;
	p->val.ival = -1;
	p->name = NULL;
	p->childs = p->next = NULL;

	if(tokenType == NONTERMINAL){
		p->name = (char*)malloc(strlen(text)+1);
		strcpy(p->name, text);
	}
	else if(tokenType == ID){
		p->name = strdup("ID");
		p->val.sval = (char*)malloc(strlen(text)+1);
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
    else if(tokenType == TYPE){   // sval存类型
        p->name = strdup("TYPE");
        // char* temp = text;
        // int i = 0;
        // while((char)*temp != 0){
        // 	*temp = toupper(*temp);
        // 	temp++;
        // }
        p->val.sval = (char*)malloc(strlen(text));
        strcpy(p->val.sval, text);
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

	return p;
}




