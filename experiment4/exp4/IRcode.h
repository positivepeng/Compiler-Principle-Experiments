#include "symbol.h"
#define REGISTERMAXLEN 20

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

void newIRcode(char opIn[10], char tIn[REGISTERMAXLEN], char a1In[REGISTERMAXLEN], char a2In[REGISTERMAXLEN], code_table* ct);
void reset(char op[10], char target[REGISTERMAXLEN], char arg1[REGISTERMAXLEN], char arg2[REGISTERMAXLEN]);

void printOutCodeTable(code_table* ct);
void printOutInterCode(code_table* ct);
void translateCond(node* root, int labelTrue, int labelFalse, symbol_table* stable, code_table* ctable, int* registerNum, int* labelNum);
int translateExp(node* root, symbol_table* stable, code_table* ctable, int* registerNum, int* labelNum);
int translateStmt(node* root, symbol_table* stable, code_table* ctable, int* registerNum, int* labelNum);
void translateFunDec(node* root, symbol_table* stable, code_table* ctable, int* registerNum);
void generateInterCode(node* root, symbol_table* stable, code_table* ctable, int* registerNum, int* labelNum);