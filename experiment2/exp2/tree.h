#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "project2.tab.h"
#define NONTERMINAL -1
#define EMPTYNODETOKEN -2

union VAL{
	int ival;
	float fval;
	char* sval;
};

struct NODE
{
	int lineBegin, lineEnd;
	int tokenType;
	union VAL val;
	char* name;
	struct NODE* childs, *next;
};

typedef struct NODE node;

void printNode(int numOfTab, node* p);
void dfsTraverse(int numOfTab, node* root);
void addChild(int numOfChild, ...);
node* newNode(int tokenType, char* text, struct YYLTYPE* loc);



