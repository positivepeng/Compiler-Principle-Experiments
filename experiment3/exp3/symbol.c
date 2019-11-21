#include "symbol.h"

symbol* findSymbolInTable(char* name, symbol_table* st);
void parseExp(node* exp, symbol_table* st);

void append2symTable(symbol_type typeIn, char* name, union VAL valIn, symbol_table* st){
	(st->symbols)[st->totalCnt].name = malloc(strlen(name)+1);
	strcpy((st->symbols)[st->totalCnt].name, name);
	(st->symbols)[st->totalCnt].type = typeIn;
	(st->symbols)[st->totalCnt].val = valIn;
	st->totalCnt++;
}

void parseDec(symbol_type typeIn, node* dec, symbol_table* st){
	// 	Dec : 	VarDec
	//			VarDec ASSIGNOP Exp[INT|FLOAT]
	//	VarDec : 	ID
	//				VarDec LB INT RB
	if(dec == NULL)
		return ;
	node* varDec = dec->childs;
	if(findSymbolInTable(varDec->childs->val.sval, st) != NULL){
		printf("Error at line %d:  Redefined variable \'%s\'\n", varDec->lineBegin, varDec->childs->val.sval);
		exit(0);
	}
	// parseVarDec(typeIn, var, st);
	union VAL val;

	// st->totalCnt++;
	if(varDec->next == NULL){
		// Dec : 	VarDec[ID]
		if(typeIn == INTNAME)
			val.ival = DEFAULTINT;
		else if(typeIn == FLOATNAME)
			val.fval = DEFAULTFLOAT;
	}
	else if(strcmp(varDec->next->name, "ASSIGNOP") == 0){
		// VarDec ASSIGNOP Exp[INT|FLOAT]
		node* exp = varDec->next->next;
		parseExp(exp, st);
		if(typeIn == INTNAME){
			if(exp->tokenType != INTNAME){
				printf("Error at line %d : Type mismatched for assignment\n", exp->childs->lineBegin);
				exit(0);
			}
			val.ival = exp->val.ival;
		}
		else if(typeIn == FLOATNAME){
			if(exp->tokenType != FLOATNAME){
				printf("Error at line %d : Type mismatched for assignment\n", exp->childs->lineBegin);
				exit(0);
			}
			val.fval = exp->val.fval;
		}
	}
	append2symTable(typeIn, varDec->childs->val.sval, val, st);
}

void parseDecList(symbol_type typeIn, node* root, symbol_table* st){
	// 	DecList :	Dec 
	// 				Dec COMMA DecList
	if(root == NULL)
		return ;
	parseDec(typeIn, root->childs, st);
	if(root->childs->next == NULL)
		return ;
	else if(strcmp(root->childs->next->name, "COMMA") == 0)
		parseDecList(typeIn, root->childs->next->next, st);
}

void parseStruct(symbol_type typeIn, node* root, symbol_table* st){
	// StructSpecifier： 	STRUCT OptTag LC DefList RC
	// OptTag： 			ID | empty
	node* optTag = root->next;
	if(optTag->childs->name == NULL)
		return ;
	union VAL tempVal;
	append2symTable(typeIn, optTag->childs->val.sval, tempVal, st);
}

void parseFuncDec(symbol_type typeIn, node* root, symbol_table* st){
	// FunDec : ID LP VarList RP
	// 			ID LP RP
	symbol* sym = findSymbolInTable(root->childs->val.sval, st);
	if(sym != NULL){
		printf("Error at line %d:  Redefined function \'%s\'\n", root->childs->lineBegin, root->childs->val.sval);
		exit(0);
	}
	union VAL tempVal;
	append2symTable(typeIn, root->childs->val.sval, tempVal, st);
}

void parseExp(node* exp, symbol_table* st){
	// exp的tokenType本来为-1,现在用来存exp表达的数据的类型
	// Exp :	Exp ASSIGNOP Exp
	// 			Exp AND Exp
	// 			Exp OR Exp
	// 			Exp RELOP Exp
	// 			LP Exp RP
	// 			MINUS Exp
	// 			NOT Exp
	// 			ID LP Args RP
	// 			ID LP RP
	// 			Exp LB Exp RB
	// 			Exp DOT ID
	if(strcmp(exp->childs->name, "INT") == 0){
		// 	Exp:	INT
		exp->tokenType = (int)INTNAME;
		exp->val.ival = exp->childs->val.ival;
	}
	else if(strcmp(exp->childs->name, "FLOAT") == 0){
		// 	Exp:	FLOAT
		exp->tokenType = (int)FLOATNAME;
		exp->val.fval = exp->childs->val.fval;
	}
	else if(strcmp(exp->childs->name, "ID") == 0){
		//	Exp:	ID
		symbol* sym = findSymbolInTable(exp->childs->val.sval, st);
		if(sym == NULL){
			printf("Error at line %d : Undefined variable \'%s\'\n", exp->childs->lineBegin, exp->childs->val.sval);
			exit(0);
		}
		exp->tokenType = (int)sym->type;
		if(sym->type == INTNAME)
			exp->val.ival = sym->val.ival;
		else if(sym->type == FLOATNAME)
			exp->val.fval = sym->val.fval;
		else {
			printf("Error at line %d : Invalid operand, only int and float are allowed\n", exp->lineBegin);
			exit(0);
		}
	}
	else if(strcmp(exp->childs->next->name, "PLUS") == 0){
		//	Exp:	Exp PLUS Exp
		node* exp1 = exp->childs, *exp2 = exp->childs->next->next;
		parseExp(exp1, st);
		parseExp(exp2, st);
		if(exp1->tokenType != exp2->tokenType){
			printf("Error at line %d : Invalid Exp\n", exp->lineBegin);
			exit(0);
		}
		if(exp1->tokenType == (int)INTNAME){
			exp->tokenType = (int)INTNAME;
			exp->val.ival = exp1->val.ival + exp2->val.ival;
		}
		else if(exp1->tokenType == (int)FLOATNAME){
			exp->tokenType = (int)FLOATNAME;
			exp->val.fval = exp1->val.fval + exp2->val.fval;
		}
	}
	else if(strcmp(exp->childs->next->name, "MINUS") == 0){
		//	Exp:	Exp MINUS Exp
		node* exp1 = exp->childs, *exp2 = exp->childs->next->next;
		parseExp(exp1, st);
		parseExp(exp2, st);
		if(exp1->tokenType != exp2->tokenType){
			printf("Error at line %d : Invalid Exp\n", exp->lineBegin);
			exit(0);
		}
		if(exp1->tokenType == (int)INTNAME){
			exp->tokenType = (int)INTNAME;
			exp->val.ival = exp1->val.ival - exp2->val.ival;
		}
		else if(exp1->tokenType == (int)FLOATNAME){
			exp->tokenType = (int)FLOATNAME;
			exp->val.fval = exp1->val.fval - exp2->val.fval;
		}
	}
	else if(strcmp(exp->childs->next->name, "STAR") == 0){
		//	Exp:	Exp STAR Exp
		node* exp1 = exp->childs, *exp2 = exp->childs->next->next;
		parseExp(exp1, st);
		parseExp(exp2, st);
		if(exp1->tokenType != exp2->tokenType){
			printf("Error at line %d : Invalid Exp\n", exp->lineBegin);
			exit(0);
		}
		if(exp1->tokenType == (int)INTNAME){
			exp->tokenType = (int)INTNAME;
			exp->val.ival = exp1->val.ival * exp2->val.ival;
		}
		else if(exp1->tokenType == (int)FLOATNAME){
			exp->tokenType = (int)FLOATNAME;
			exp->val.fval = exp1->val.fval * exp2->val.fval;
		}
	}
	else if(strcmp(exp->childs->next->name, "DIV") == 0){
		//	Exp:	Exp DIV Exp
		node* exp1 = exp->childs, *exp2 = exp->childs->next->next;
		parseExp(exp1, st);
		parseExp(exp2, st);
		if(exp1->tokenType != exp2->tokenType){
			printf("Error at line %d : Invalid Exp\n", exp->lineBegin);
			exit(0);
		}
		if(exp1->tokenType == (int)INTNAME){
			exp->tokenType = (int)INTNAME;
			exp->val.ival = exp1->val.ival / exp2->val.ival;
		}
		else if(exp1->tokenType == (int)FLOATNAME){
			exp->tokenType = (int)FLOATNAME;
			exp->val.fval = exp1->val.fval / exp2->val.fval;
		}
	}
	else if(strcmp(exp->childs->next->name, "ASSIGNOP") == 0){
		//	Exp:	Exp ASSIGNOP Exp
		node* exp1 = exp->childs, *exp2 = exp->childs->next->next;
		parseExp(exp1, st);
		parseExp(exp2, st);

		exp->tokenType = exp1->tokenType = exp2->tokenType;
		if(exp2->tokenType == (int)INTNAME){
			exp->val.ival = exp1->val.ival = exp2->val.ival;
		}
		else if(exp2->tokenType == (int)FLOATNAME){
			exp->val.fval = exp1->val.fval = exp2->val.fval;
		}
		
		if(strcmp(exp1->childs->name, "ID") == 0){
			symbol* sym = findSymbolInTable(exp1->childs->val.sval, st);
			if((int)sym->type != exp2->tokenType){
				printf("Error at line %d : Type mismatched for assignment\n", exp->lineBegin);
			}
			if(exp2->tokenType == (int)INTNAME){
				sym->val.ival = exp1->val.ival;
			}
			else if(exp2->tokenType == (int)FLOATNAME){
				sym->val.fval = exp1->val.fval;
			}
		}
	}
}

void parseAllExp(node* root, symbol_table* st){
	// 对所有的exp进行求值
	if(root == NULL || root->name == NULL)
		return ;

	if(strcmp(root->name, "Exp") == 0){
		parseExp(root, st);
	}

	// 遍历整棵树
	if(root->childs != NULL){
		node* sibling = root->childs;
		while(sibling != NULL){
			parseAllExp(sibling, st);
			sibling = sibling->next;
		}
	}
}

void saveSymbol2table(node* root, symbol_table* st){
	if(root == NULL || root->name == NULL)
		return ;

	symbol_type type;

	if(strcmp(root->name, "Def") == 0){
		// 局部变量
		// 	Def : 		Specifier[TYPE|StructSpecifer[STRUCT]] DecList SEMI
		node* spec = root->childs;
		assert(strcmp(spec->name, "Specifier") == 0);

		if(strcmp(spec->childs->name, "TYPE") == 0){
			node* typeNode = spec->childs;
			if(strcmp(typeNode->val.sval, "int") == 0)
				type = INTNAME;		
			else if(strcmp(typeNode->val.sval, "float") == 0)
				type = FLOATNAME;
			// DecList
			node* dec = spec->next->childs;
			while(dec != NULL){
				if(strcmp(dec->name, "Dec") == 0){
					parseDec(type, dec, st);
				}
				if(strcmp(dec->name, "DecList") == 0){
					parseDecList(type, dec, st);
				}
				dec = dec->next;
			}
		}
	}
	else if(strcmp(root->name, "ExtDef") == 0){
		// 全局变量
		// ExtDef : Specifier[TYPE|StructSpecifer[STRUCT]] ExtDecList SEMI
		//			Specifier SEMI
		// 			Specifier FunDec CompSt
		node* spec = root->childs;
		assert(strcmp(spec->name, "Specifier") == 0);

		if(strcmp(spec->next->name, "ExtDecList") == 0){
			// 全局变量  先不考虑数组
			// ExtDecList	VarDec
			// 				VarDec COMMA ExtDecList
		}
		else if(strcmp(spec->next->name, "SEMI") == 0){
			// 结构体，或空
			// Specifier： 	TYPE
			// 				StructSpecifier
			// StructSpecifier： 	STRUCT OptTag LC DefList RC
			// 						STRUCT Tag(声明时忽略)
			// OptTag： 			ID | empty
			// Tag： ID
			if(strcmp(spec->childs->name, "StructSpecifier") == 0){
				type = STRUCTNAME;
				parseStruct(type, spec->childs->childs, st);
			}
			else if(strcmp(spec->childs->name, "TYPE") == 0){
				printf("Empty Declare!\n");
			}
			
		}
		else if(strcmp(spec->next->name, "FunDec") == 0){
			// 函数定义
			// FunDec : ID LP VarList RP
			// 			ID LP RP
			type = FUNCTIONNAME;
			parseFuncDec(type, spec->next, st);
		}
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

void printOutTable(symbol_table* st){
	printf("PrintOut Symbol Table\n");
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

symbol* findSymbolInTable(char* name, symbol_table* st){
	// 线性查找
	for(int i = 0;i < st->totalCnt; i++){
		if(strcmp(name, (st->symbols)[i].name) == 0)
			return &(st->symbols)[i];
	}
	return NULL;
}