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

symbol* findSymbolInTable(char* name, symbol_table* st);
void parseExp(node* exp, symbol_table* st);

void append2symTable(symbol_type typeIn, char* name, union VAL valIn, symbol_table* st);

void parseDec(symbol_type typeIn, node* dec, symbol_table* st);

void parseDecList(symbol_type typeIn, node* root, symbol_table* st);

void parseStruct(symbol_type typeIn, node* root, symbol_table* st);

void parseFuncDec(symbol_type typeIn, node* root, symbol_table* st);

void parseAllExp(node* root, symbol_table* st);

void saveSymbol2table(node* root, symbol_table* st);

void printOutTable(symbol_table* st);

void freeTableMemory(symbol_table* st);