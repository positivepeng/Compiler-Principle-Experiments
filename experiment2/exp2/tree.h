#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct NODE
{
	int rowNum, colNum;
	int val;
	char* name;
	struct NODE* childs, *next, *par;
};
typedef struct NODE node;