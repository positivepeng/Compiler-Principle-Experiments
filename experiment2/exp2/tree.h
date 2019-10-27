#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project2.tab.h"

union VAL{
	int ival;
	float fval;
	char* sval;
};

struct NODE
{
	int lineNum, colNum;
	union VAL val;
	char* name;
	struct NODE* childs, *next, *par;
};

typedef struct NODE node;
