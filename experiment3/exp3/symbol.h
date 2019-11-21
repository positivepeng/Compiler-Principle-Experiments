#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define DEFAULTINT 0
#define DEFAULTFLOAT 0.0

typedef enum
{
	INTNAME=1, FLOATNAME, FUNCTIONNAME, STRUCTNAME, INTARRAYNAME, FLOATARRAYNAME, UNCLEAR
}symbol_type;

typedef struct 
{
	char* name;
	symbol_type type;
	union VAL val;
}symbol;

typedef struct 
{
	symbol symbols[100];
	int totalCnt;
}symbol_table;

