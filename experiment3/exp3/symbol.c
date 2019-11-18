#include "symbol.h"

void saveSymbol2table(node* root, symbol_table* st){
	if(root == NULL || root->name == NULL)
		return ;

	enum symbol_type type;

	if(strcmp(root->name, "Def") == 0){
		// 局部变量
		// 	Def : 		Specifier[TYPE|StructSpecifer[STRUCT]] DecList SEMI
		// 	DecList :	Dec 
		// 				Dec COMMA DecList
		// 	Dec : 	VarDec
		//			VarDec ASSIGNOP Exp
		//	VarDec : 	ID
		//				VarDec LB INT RB
		printf("Def at : %d\n", root->lineBegin);

		node* spec = root->childs;
		assert(strcmp(spec->name, "Specifier") == 0);

		if(strcmp(spec->childs->name, "TYPE") == 0){
			node* typeNode = spec->childs;
			if(strcmp(typeNode->val.sval, "int") == 0)
				type = INTNAME;		
			else if(strcmp(typeNode->val.sval, "float") == 0)
				type = FLOATNAME;
		}
		printf("Def type:%d\n", type);
	}
	if(strcmp(root->name, "ExtDef") == 0){
		// 全局变量
		// ExtDef : Specifier[TYPE|StructSpecifer[STRUCT]] ExtDecList SEMI
		//			Specifier SEMI
		// 			Specifier FunDec CompSt
		printf("ExtDef at: %d\n", root->lineBegin);
		node* spec = root->childs;
		assert(strcmp(spec->name, "Specifier") == 0);

		if(strcmp(spec->next->name, "ExtDecList") == 0){
			// 全局变量
		}
		else if(strcmp(spec->next->name, "SEMI") == 0){
			// 结构体，或空
		}
		else if(strcmp(spec->next->name, "FunDec") == 0){
			// 函数定义
		}
		printf("ExtDef type: %d\n", type);
	}

	// 遍历整棵树
	if(root->childs != NULL){
		node* sibling = root->childs;
		while(sibling != NULL){
			saveSymbol2table(sibling, st);
			sibling = sibling->next;
		}
	}
}


symbol* node2symbol(node* nodein, enum symbol_type type){
	symbol* sym = malloc(sizeof(symbol));
	sym->name = strdup(nodein->name);
	if(nodein->tokenType == INT){
		sym->type = INTNAME;
		sym->val.ival = nodein->val.ival;
	}
	else if(nodein->tokenType == FLOAT){
		sym->type = FLOATNAME;
		sym->val.fval = nodein->val.fval;	
	}
	else{
		sym->type = UNCLEAR;
		if(nodein->val.sval != NULL)
			sym->val.sval = strdup(nodein->val.sval);
		else
			sym->val.sval = NULL;	
	}
	return sym;
}

void printOutTable(symbol_table* st){
	for(int i = 0;i < st->totalCnt; i++){
		printf("name : %s type: %d ", (st->symbols)[i].name, (st->symbols)[i].type);
		if((st->symbols)[i].type == INTNAME)
			printf("val : %d\n", (st->symbols)[i].val.ival);
		else if((st->symbols)[i].type == FLOATNAME)
			printf("val : %f\n", (st->symbols)[i].val.fval);
		else
			printf("val : %s\n", (st->symbols)[i].val.sval);
	}
}

symbol* find(char* name, symbol_table* st){
	// 线性查找
	for(int i = 0;i < st->totalCnt; i++){
		if(strcmp(name, (st->symbols)[i].name) == 0)
			return &(st->symbols)[i];
	}
	return NULL;
}

void append(symbol_table* st, symbol* newsym){
	// st[st->totalCnt] = newsym;
	(st->symbols)[st->totalCnt].name = strdup(newsym->name);
	(st->symbols)[st->totalCnt].type = newsym->type;
	(st->symbols)[st->totalCnt].val = newsym->val;
	st->totalCnt++;
}

void test_append_find_printOut(){
	symbol_table st;
	symbol ns1, ns2, ns3;

	ns1.name = strdup("INT");
	ns1.type = INTNAME;
	ns1.val.ival = 23;

	ns2.name = strdup("FLOAT");
	ns2.type = FLOATNAME;
	ns2.val.fval = 23.3;

	ns3.name = strdup("UNCLEAR");
	ns3.type = UNCLEAR;
	ns3.val.sval = NULL;

	append(&st, &ns1);
	append(&st, &ns2);
	append(&st, &ns3);

	printf("%d\n", st.totalCnt);

	symbol* ans = find("Hello", &st);

	if(ans == NULL)
		printf("not found \n");
    else
		printf("%s %d %d\n", ans->name, ans->type, ans->val.ival);

	printOutTable(&st);

	free(ns1.name);
	free(ns2.name);
	free(ns3.name);
}

void test_node2symbol(){
	node nodein;
	nodein.name = strdup("hello");
	nodein.val.ival = 23;
	nodein.tokenType = ID;
	nodein.val.sval = NULL;
	symbol* sym = node2symbol(&nodein, INTNAME);
	printf("%s %d %s \n", sym->name, sym->type, sym->val.sval);
	free(sym->name);
	free(sym);
}


// int main(int argc, char const *argv[])
// {
// 	test_append_find_printOut();
// 	// test_node2symbol();
// 	return 0;
// }