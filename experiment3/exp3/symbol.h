#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
enum symbol_type
{
	INTNAME=1, FLOATNAME, FUNCTIONNAME, STRUCTNAME, INTARRAYNAME, FLOATARRAYNAME, UNCLEAR
};

typedef struct 
{
	char* name;
	enum symbol_type type;
	union VAL val;
}symbol;

typedef struct 
{
	symbol symbols[100];
	int totalCnt;
}symbol_table;

