#include "project2.tab.h"
#include <string.h>

struct NODE{
	int rownum, colnum;
	int val;
	char* name;
	struct NODE* childs, *next, *par;
};

typedef struct NODE node