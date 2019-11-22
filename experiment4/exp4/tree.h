#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "project3.tab.h"
#define NONTERMINAL -1
#define EMPTYNODETOKEN -2

union VAL{
	int ival;
	float fval;
	char* sval;
	// structInfo* stval;   // 结构体定义包含的信息
	// funcInfo* funcval;	// 函数定义包含的信息
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