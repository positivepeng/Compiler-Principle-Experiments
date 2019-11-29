#include "symbol.h"
#define REGISTERMAXLEN 5

// 单个域
typedef struct
{
	symbol_type type;
	char name[100];
}field;

typedef struct
{
	char target[REGISTERMAXLEN];
	char arg1[REGISTERMAXLEN];
	char arg2[REGISTERMAXLEN];
	char op[10];   // 如果为函数调用则op为函数名
}inter_code;

typedef struct {
	int totalCnt;
	inter_code codes[10000];
}code_table;
