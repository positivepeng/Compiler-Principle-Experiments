#include "symbol.h"

void append2symTable(symbol_type typeIn, char* name, union VAL valIn, symbol_table* st){
	(st->symbols)[st->totalCnt].name = malloc(strlen(name)+1);
	strcpy((st->symbols)[st->totalCnt].name, name);
	(st->symbols)[st->totalCnt].type = typeIn;
	(st->symbols)[st->totalCnt].val = valIn;
	st->totalCnt++;
}

void parseDec(symbol_type typeIn, node* dec, symbol_table* st, char* structName){
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
	union VAL val;

	if(varDec->next == NULL){
		// Dec : 	VarDec[ID]
		if(typeIn == INTNAME)
			val.ival = DEFAULTINT;
		else if(typeIn == FLOATNAME)
			val.fval = DEFAULTFLOAT;
		else if(typeIn == STRUCTNAME){
			symbol* sym = findSymbolInTable(structName, st);
			if(sym == NULL){
				printf("Error at line %d : Undefined Struct \'%s\'\n", varDec->lineBegin, structName);
				exit(0);
			}
			val.sval = malloc(strlen(structName)+1);
			strcpy(val.sval, structName);
		}
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

void parseDecList(symbol_type typeIn, node* root, symbol_table* st, char* structName){
	// 	DecList :	Dec 
	// 				Dec COMMA DecList
	if(root == NULL)
		return ;
	parseDec(typeIn, root->childs, st, structName);
	if(root->childs->next == NULL)
		return ;
	else if(strcmp(root->childs->next->name, "COMMA") == 0)
		parseDecList(typeIn, root->childs->next->next, st, structName);
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
	// Exp :	Exp RELOP Exp
	// 			LP Exp RP
	// 			MINUS Exp
	// 			NOT Exp
	// 			Exp LB Exp RB
	
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
	}
	else if(strcmp(exp->childs->next->name, "AND") == 0){
		// Exp AND Exp
		node* exp1 = exp->childs, *exp2 = exp->childs->next->next;
		parseExp(exp1, st);
		parseExp(exp2, st);
		if(exp1->tokenType != exp2->tokenType){
			printf("Error at line %d : Type mismatched for \'&&\'\n", exp->lineBegin);
			exit(0);
		}
		else{
			exp->tokenType = exp1->tokenType;
		}
	}
	else if(strcmp(exp->childs->next->name, "OR") == 0){
		// Exp OR Exp
		node* exp1 = exp->childs, *exp2 = exp->childs->next->next;
		parseExp(exp1, st);
		parseExp(exp2, st);
		if(exp1->tokenType != exp2->tokenType){
			printf("Error at line %d : Type mismatched for \'||\'\n", exp->lineBegin);
			exit(0);
		}
		else{
			exp->tokenType = exp1->tokenType;
		}
	}
	else if(strcmp(exp->childs->next->name, "ASSIGNOP") == 0){
		//	Exp:	Exp ASSIGNOP Exp
		node* exp1 = exp->childs, *exp2 = exp->childs->next->next;
		parseExp(exp1, st);
		parseExp(exp2, st);

		if(exp1->tokenType != exp2->tokenType){
			printf("Error at line %d : Type mismatched for assignment\n", exp->lineBegin);
			exit(0);
		}
		exp->tokenType = exp1->tokenType;
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
	else if(strcmp(exp->childs->next->name, "DOT") == 0){
		//Exp:	Exp DOT ID
		symbol* sym = findSymbolInTable(exp->childs->childs->val.sval, st);
		if(sym == NULL){
			printf("Error at line %d : Undefined Struct \'%s\'\n", exp->lineBegin,\
			 exp->childs->childs->val.sval);
			exit(0);
		}
		else if(sym->type != STRUCTNAME || sym->cnt != 0){
			//　不允许空的结构体
			//　sym->cnt == 0 则表示该ID为结构体名
			printf("Error at line %d : \'%s\' \'s type is not struct\n", exp->lineBegin,\
										 exp->childs->childs->val.sval);
			exit(0);
		}
		int fieldType, flag = 0;
		symbol* stSym = findSymbolInTable(sym->val.sval,st);
		for(int i = 0;i < stSym->cnt; i++){
			if(strcmp((stSym->fieldName)[i], exp->childs->next->next->val.sval) == 0){
				flag = 1;
				fieldType = (stSym->paramType)[i];
				break;
			}
		}
		if(flag == 0){
			printf("Error at line %d : struct \'%s\' has no filed named \'%s\'\n",\
			 exp->lineBegin, sym->val.sval, exp->childs->next->next->val.sval);
			exit(0);
		}
		else{
			exp->tokenType = fieldType;
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

void parseVarList(node* varList, int* paramCnt, symbol_type* paramType, char** fieldName){
	// VarList : 	ParamDec COMMA VarList
	// 				ParamDec
	if(varList == NULL)
		return ;
	// ParamDec : Specifier[TYPE] VarDec[ID]
	paramType[*paramCnt] = strcmp(varList->childs->childs->childs->val.sval, "int") == 0 ? INTNAME : FLOATNAME;
	fieldName[*paramCnt] = malloc(strlen(varList->childs->childs->next->childs->val.sval)+1);
	strcpy(fieldName[*paramCnt], varList->childs->childs->next->childs->val.sval);
	(*paramCnt)++;
	
	if(varList->childs->next != NULL)
		parseVarList(varList->childs->next->next, paramCnt, paramType, fieldName);
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
		// Def : 		Specifier[TYPE|StructSpecifer] DecList SEMI
		// StructSpecifier： 	STRUCT OptTag[ID] LC DefList RC    // 假设不会有这种情况
		// 						STRUCT Tag[ID]
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
					parseDec(type, dec, st, NULL);
				}
				if(strcmp(dec->name, "DecList") == 0){
					parseDecList(type, dec, st, NULL);
				}
				dec = dec->next;
			}
		}
		else if(strcmp(spec->childs->name, "StructSpecifier") == 0){
			if(strcmp(spec->childs->childs->next->name, "Tag") == 0){
				char* structName = spec->childs->childs->next->childs->val.sval;
				type = STRUCTNAME;
				// DecList
				node* dec = spec->next->childs;
				while(dec != NULL){
					if(strcmp(dec->name, "Dec") == 0){
						parseDec(type, dec, st, structName);
					}
					if(strcmp(dec->name, "DecList") == 0){
						parseDecList(type, dec, st, structName);
					}
					dec = dec->next;
				}
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
			// StructSpecifier： 	STRUCT OptTag[ID] LC DefList RC
			// 						STRUCT Tag[ID]
			if(strcmp(spec->childs->name, "StructSpecifier") == 0){
				if(strcmp(spec->childs->childs->next->name, "OptTag") == 0){
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

			// 设置返回值类型
			if(strcmp(spec->childs->val.sval, "int") == 0)
				sym->returnValType = INTNAME;
			else if(strcmp(spec->childs->val.sval, "float") == 0)
				sym->returnValType = FLOATNAME;
			if(strcmp(spec->next->childs->next->next->name, "VarList") == 0){
				// FunDec : ID LP VarList RP
				// 默认返回值和参数只有int或者float
				parseVarList(spec->next->childs->next->next, &(sym->cnt), sym->paramType, sym->fieldName);	
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
	printf("%8s\t%8s\t%8s\n", "name", "type", "info");
	for(int i = 0;i < st->totalCnt; i++){
		printf("%8s\t", (st->symbols)[i].name);
		if((st->symbols)[i].type == INTNAME)
			printf("%8s\t%8d\n", "int", (st->symbols)[i].val.ival);
		else if((st->symbols)[i].type == FLOATNAME)
			printf("%8s\t%8f\n", "float", (st->symbols)[i].val.fval);
		else if((st->symbols)[i].type == STRUCTNAME){
			printf("%8s", "struct");
			if((st->symbols)[i].cnt == 0){
				printf("\tstruct %s\n", (st->symbols)[i].val.sval);
			}
			else{
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
			
		}
		else if((st->symbols)[i].type == FUNCTIONNAME){
			printf("%8s", "function");
			printf("\tparam cnt : %d ", (st->symbols)[i].cnt);
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