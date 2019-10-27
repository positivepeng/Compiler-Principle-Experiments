#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project2.tab.h"

union VAL{
	int ival;
	float fval;
};

struct NODE
{
	int rowNum, colNum;
	union VAL val;
	char* name;
	struct NODE* childs, *next, *par;
};
typedef struct NODE node;