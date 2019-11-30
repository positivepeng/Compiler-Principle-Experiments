#include "symbol.h"

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

int checkArgs(symbol* sym, node* args,symbol_table* st, int currCnt){
	// Args : 	Exp COMMA Args
	// 			Exp
	node* exp = args->childs;
	parseExp(exp, st);

	if(exp->tokenType == (sym->paramType)[currCnt]){
		currCnt++;
		if(exp->next == NULL){
			return currCnt == sym->cnt;
		}
		else 
			return checkArgs(sym, exp->next->next, st, currCnt);
	}
	else 
		return 0;
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
	else if(strcmp(exp->childs->name, "ID") == 0 && exp->childs->next == NULL){
		//	Exp:	ID (不能是函数调用)
		symbol* sym = findSymbolInTable(exp->childs->val.sval, st);
		if(sym == NULL){
			printf("Error at line %d : Undefined variable \'%s\'\n", exp->childs->lineBegin, exp->childs->val.sval);
			exit(0);
		}
		// if(sym->type == INTNAME)
		// 	exp->val.ival = sym->val.ival;
		// else if(sym->type == FLOATNAME)
		// 	exp->val.fval = sym->val.fval;
		if(sym->type == INTNAME || sym->type == FLOATNAME)
			exp->tokenType = (int)sym->type;
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
			printf("Error at line %d : Type mismatched for \'+\'\n", exp->lineBegin);
			exit(0);
		}
		else{
			exp->tokenType = exp1->tokenType;
		}
		// if(exp1->tokenType == (int)INTNAME){
		// 	exp->tokenType = (int)INTNAME;
		// 	exp->val.ival = exp1->val.ival + exp2->val.ival;
		// }
		// else if(exp1->tokenType == (int)FLOATNAME){
		// 	exp->tokenType = (int)FLOATNAME;
		// 	exp->val.fval = exp1->val.fval + exp2->val.fval;
		// }
	}
	else if(strcmp(exp->childs->next->name, "MINUS") == 0){
		//	Exp:	Exp MINUS Exp
		node* exp1 = exp->childs, *exp2 = exp->childs->next->next;
		parseExp(exp1, st);
		parseExp(exp2, st);
		if(exp1->tokenType != exp2->tokenType){
			printf("Error at line %d : Type mismatched for \'-\'\n", exp->lineBegin);
			exit(0);
		}
		else{
			exp->tokenType = exp1->tokenType;
		}
		// if(exp1->tokenType == (int)INTNAME){
		// 	exp->tokenType = (int)INTNAME;
		// 	exp->val.ival = exp1->val.ival - exp2->val.ival;
		// }
		// else if(exp1->tokenType == (int)FLOATNAME){
		// 	exp->tokenType = (int)FLOATNAME;
		// 	exp->val.fval = exp1->val.fval - exp2->val.fval;
		// }
	}
	else if(strcmp(exp->childs->next->name, "STAR") == 0){
		//	Exp:	Exp STAR Exp
		node* exp1 = exp->childs, *exp2 = exp->childs->next->next;
		parseExp(exp1, st);
		parseExp(exp2, st);
		if(exp1->tokenType != exp2->tokenType){
			printf("Error at line %d : Type mismatched for \'*\'\n", exp->lineBegin);
			exit(0);
		}
		else{
			exp->tokenType = exp1->tokenType;
		}
		// if(exp1->tokenType == (int)INTNAME){
		// 	exp->tokenType = (int)INTNAME;
		// 	exp->val.ival = exp1->val.ival * exp2->val.ival;
		// }
		// else if(exp1->tokenType == (int)FLOATNAME){
		// 	exp->tokenType = (int)FLOATNAME;
		// 	exp->val.fval = exp1->val.fval * exp2->val.fval;
		// }
	}
	else if(strcmp(exp->childs->next->name, "DIV") == 0){
		//	Exp:	Exp DIV Exp
		node* exp1 = exp->childs, *exp2 = exp->childs->next->next;
		parseExp(exp1, st);
		parseExp(exp2, st);
		if(exp1->tokenType != exp2->tokenType){
			printf("Error at line %d : Type mismatched for \'/\'\n", exp->lineBegin);
			exit(0);
		}
		else{
			exp->tokenType = exp1->tokenType;
		}
		// if(exp1->tokenType == (int)INTNAME){
		// 	exp->tokenType = (int)INTNAME;
		// 	//exp->val.ival = exp1->val.ival / exp2->val.ival;
		// }
		// else if(exp1->tokenType == (int)FLOATNAME){
		// 	exp->tokenType = (int)FLOATNAME;
		// 	exp->val.fval = exp1->val.fval / exp2->val.fval;
		// }
	}
	else if(strcmp(exp->childs->next->name, "ASSIGNOP") == 0){
		//	Exp:	Exp ASSIGNOP Exp
		node* exp1 = exp->childs, *exp2 = exp->childs->next->next;
		parseExp(exp1, st);
		parseExp(exp2, st);
		// if(exp2->tokenType == (int)INTNAME){
		// 	exp->val.ival = exp1->val.ival = exp2->val.ival;
		// }
		// else if(exp2->tokenType == (int)FLOATNAME){
		// 	exp->val.fval = exp1->val.fval = exp2->val.fval;
		// }
		
		if(strcmp(exp1->childs->name, "ID") == 0){
			symbol* sym = findSymbolInTable(exp1->childs->val.sval, st);
			if((int)sym->type != exp2->tokenType){
				printf("Error at line %d : Type mismatched for assignment\n", exp->lineBegin);
				exit(0);
			}
			else
				exp->tokenType = (int)sym->type;
			// if(exp2->tokenType == (int)INTNAME){
			// 	sym->val.ival = exp1->val.ival;
			// }
			// else if(exp2->tokenType == (int)FLOATNAME){
			// 	sym->val.fval = exp1->val.fval;
			// }
		}
		else{
			printf("Error at line %d : Invalid Left Value\n", exp->lineBegin);
		}
	}
	else if(strcmp(exp->childs->next->name, "LP") == 0){
		//Exp:	ID LP Args RP
		// 		ID LP RP
		// 检查函数是否存在
		char* fname = exp->childs->val.sval;
		symbol* sym = findSymbolInTable(fname, st);
		if(sym == NULL){
			printf("Error at line %d : Undefined Function \'%s\'\n", exp->lineBegin, fname);
			exit(0);
		}
		else if(sym->type != FUNCTIONNAME){
			printf("Error at line %d : \'%s\' is not a function \n", exp->lineBegin, fname);
			exit(0);
		}

		exp->tokenType = (int)(sym->returnValType);
		// 无参数函数
		if(strcmp(exp->childs->next->next->name, "RP") == 0 && sym->cnt == 0){
			return ;
		}
		// 有参数函数
		if(strcmp(exp->childs->next->next->name, "Args") == 0 && checkArgs(sym, exp->childs->next->next, st, 0) == 1){
			return ;
		}
		printf("Error at line %d : Type mismatched for function parameters\n", exp->lineBegin);
		exit(0);
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

void parseVarList(node* varList, int* paramCnt, symbol_type* paramType){
	// VarList : 	ParamDec COMMA VarList
	// 				ParamDec
	if(varList == NULL)
		return ;
	// ParamDec : Specifier[TYPE] VarDec[ID]
	paramType[*paramCnt] = strcmp(varList->childs->childs->childs->val.sval, "int") == 0 ? INTNAME : FLOATNAME;
	(*paramCnt)++;
	
	if(varList->childs->next != NULL)
		parseVarList(varList->childs->next->next, paramCnt, paramType);
}

void parseDefList(node* defList, int* paramCnt, symbol_type* paramType, char* fieldName[MAXFILEDNUM]){
	// DefList :  Def DefList
	// 			| empty
	// Def : Specifier DecList SEMI
	// DecList 	:	Dec 
	// Dec 		: 	VarDec
	// VarDec 	: 	ID
	// 结构体内只支持单个变量声明
	if(defList->tokenType == EMPTYNODETOKEN){
		return ;
	}
	else if(strcmp(defList->childs->name, "Def") == 0){
		node* spec = defList->childs->childs;
		paramType[*paramCnt] = strcmp(spec->childs->val.sval, "int") == 0 ? INTNAME : FLOATNAME;

		char* name = spec->next->childs->childs->childs->val.sval;
		fieldName[*paramCnt] = malloc(strlen(name)+1);
		strcpy(fieldName[*paramCnt], name);
		(*paramCnt)++;
	}
	parseDefList(defList->childs->next, paramCnt, paramType, fieldName);
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
			// OptTag： 			ID
			if(strcmp(spec->childs->name, "StructSpecifier") == 0){
				type = STRUCTNAME;
				// 将结构体的信息添加到符号表中
				parseStruct(type, spec->childs->childs, st);

				node* defList = spec->childs->childs->next->next->next;

				// 将结构体的信息加入到符号表中
				// DefList :  Def DefList
				// 			| empty
				// Def : Specifier DecList SEMI
				// 最新添加的符号就是结构体
				symbol* sym = &(st->symbols)[st->totalCnt-1];
				sym->cnt = 0;
				parseDefList(defList, &(sym->cnt), sym->paramType, sym->fieldName);

			}
			else if(strcmp(spec->childs->name, "TYPE") == 0){
				printf("Empty Declare!\n");
			}
			
		}
		else if(strcmp(spec->next->name, "FunDec") == 0){
			// 函数定义
			// ExtDef :	Specifier FunDec CompSt
			// FunDec : ID LP VarList RP
			// 			ID LP RP
			type = FUNCTIONNAME;
			parseFuncDec(type, spec->next, st);

			// 最新添加的符号就是函数
			symbol* sym = &(st->symbols)[st->totalCnt-1];

			// 初始化
			sym->cnt = 0;
			if(strcmp(spec->next->childs->next->next->name, "VarList") == 0){
				// FunDec : ID LP VarList RP
				// 默认返回值和参数只有int或者float
				if(strcmp(spec->childs->val.sval, "int") == 0)
					sym->returnValType = INTNAME;
				else if(strcmp(spec->childs->val.sval, "float") == 0)
					sym->returnValType = FLOATNAME;
				

				parseVarList(spec->next->childs->next->next, &(sym->cnt), sym->paramType);	

				// printf("parse finish: %s %d\n",(st->symbols)[st->totalCnt-1].name ,(st->symbols)[st->totalCnt-1].cnt);
			}
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
	printf("%8s\t%8s\t%8s\n", "name", "type", "info");
	for(int i = 0;i < st->totalCnt; i++){
		printf("%8s\t", (st->symbols)[i].name);
		if((st->symbols)[i].type == INTNAME)
			printf("%8s\t%8d\n", "int", (st->symbols)[i].val.ival);
		else if((st->symbols)[i].type == FLOATNAME)
			printf("%8s\t%8f\n", "float", (st->symbols)[i].val.fval);
		else if((st->symbols)[i].type == STRUCTNAME){
			printf("%8s", "struct");
			printf("\tfield cnt : %d ", (st->symbols)[i].cnt);
			for(int j = 0;j < (st->symbols)[i].cnt; j++){
				if(j == 0)
					printf("(");
				if(j > 0)
					printf(",");
				printf("%s %s", (st->symbols)[i].paramType[j] == INTNAME ? "int" : "float" ,(st->symbols)[i].fieldName[j]);
				if(j == (st->symbols)[i].cnt-1)
					printf(")");	
			}
			printf("\n");
		}
		else if((st->symbols)[i].type == FUNCTIONNAME){
			printf("%8s", "function");
			printf("\tparam cnt : %d ", (st->symbols)[i].cnt);
			for(int j = 0;j < (st->symbols)[i].cnt; j++){
				if(j == 0)
					printf("(");
				if(j > 0)
					printf(",");
				printf("%s", (st->symbols)[i].paramType[j] == INTNAME ? "int" : "float");
				if(j == (st->symbols)[i].cnt-1)
					printf(")");
			}
			printf("\n");
		}
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

int getSymbolIndex(char* name, symbol_table* st){
	for(int i = 0;i < st->totalCnt; i++){
		if(strcmp(name, (st->symbols)[i].name) == 0)
			return i;
	}
	return -1;	
}

void freeTableMemory(symbol_table* st){
	for(int i = 0;i < st->totalCnt; i++){
		if((st->symbols)[i].name != NULL)
			free((st->symbols)[i].name);
	}
}