#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define DEFAULTINT 0
#define DEFAULTFLOAT 0.0

#define MAXFILEDNUM 10
#define MAXPARAMNUM 10

typedef enum
{
	INTNAME=1, FLOATNAME, FUNCTIONNAME, STRUCTNAME, INTARRAYNAME, FLOATARRAYNAME, UNCLEAR
}symbol_type;

typedef struct 
{
	char* name;
	symbol_type type;
	union VAL val;
	int cnt;							//函数则为参数个数，结构体则为域个数
	symbol_type returnValType;			//函数返回类型
	char* fieldName[MAXFILEDNUM];		// 结构体域的名字
	symbol_type paramType[MAXPARAMNUM];	// 函数参数或结构体域的类型
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
int checkArgs(symbol* sym, node* args,symbol_table* st, int currCnt);

void parseAllExp(node* root, symbol_table* st);
void parseVarList(node* varList, int* paramCnt, symbol_type* paramType);

void parseDefList(node* defList, int* paramCnt, symbol_type* paramType, char* fieldName[MAXFILEDNUM]);

void saveSymbol2table(node* root, symbol_table* st);

void printOutTable(symbol_table* st);
int getSymbolIndex(char* name, symbol_table* st);
void freeTableMemory(symbol_table* st);