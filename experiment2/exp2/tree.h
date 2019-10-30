#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "project2.tab.h"
#define NONTERMINAL -1

union VAL{
	int ival;
	float fval;
	char* sval;
};

struct NODE
{
	int lineNum, colNum;
	int tokenType;
	union VAL val;
	char* name;
	struct NODE* childs, *next, *par;
};

typedef struct NODE node;
