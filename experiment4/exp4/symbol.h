#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define DEFAULTINT 0
#define DEFAULTFLOAT 0.0

#define MAXFILEDNUM 10
#define MAXPARAMNUM 10

// struct funcVal;
// struct structVal;

typedef enum
{
	INTNAME=1, FLOATNAME, FUNCTIONNAME, STRUCTNAME, INTARRAYNAME, FLOATARRAYNAME, UNCLEAR
}symbol_type;

typedef struct 
{
	char* name;
	symbol_type type;
	union VAL val;
	int cnt;					//函数则为参数个数，结构体则为域个数
	symbol_type returnValType;	//函数返回类型
	char* fieldName[MAXFILEDNUM];	// 结构体域的名字
	symbol_type paramType[MAXPARAMNUM];	// 函数参数或结构体域的类型
}symbol;

typedef struct 
{
	symbol symbols[100];
	int totalCnt;
}symbol_table;