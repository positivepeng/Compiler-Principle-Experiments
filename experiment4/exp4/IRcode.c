#include "IRcode.h"

void newIRcode(char opIn[10], char tIn[REGISTERMAXLEN], char a1In[REGISTERMAXLEN], char a2In[REGISTERMAXLEN], code_table* ct){
	strcpy(ct->codes[ct->totalCnt].op, opIn);
	strcpy(ct->codes[ct->totalCnt].target, tIn);
	strcpy(ct->codes[ct->totalCnt].arg1, a1In);
	strcpy(ct->codes[ct->totalCnt].arg2, a2In);
	ct->totalCnt++;
}

void reset(char op[10], char target[REGISTERMAXLEN], char arg1[REGISTERMAXLEN], char arg2[REGISTERMAXLEN]){
	memset(op, 0, 10);
	memset(target, 0, REGISTERMAXLEN);
	memset(arg1, 0, REGISTERMAXLEN);
	memset(arg2, 0, REGISTERMAXLEN);
}

void printOutCodeTable(code_table* ct){
	printf("%10s %10s %10s %10s\n", "target", "op", "arg1", "arg2");
	for(int i = 0;i < ct->totalCnt; i++){
		printf("%10s %10s %10s %10s\n", ct->codes[i].target, ct->codes[i].op, ct->codes[i].arg1, ct->codes[i].arg2);
	}
}


int translateExp(node* root, symbol_table* stable, code_table* ctable, int* registerNum){
	// 返回结果存入的寄存器的编号
	// 解析Exp
	// Exp : 	Exp ASSIGNOP Exp
	// 			Exp RELOP Exp
	// 			Exp PLUS Exp
	// 			Exp MINUS Exp
	// 			Exp STAR Exp
	// 			Exp DIV Exp
	// 			ID LP Args RP
	// 			ID LP RP
	// 			ID
	// 			INT
	// 			FLOAT
	root->isVisited = 1;
	char op[10], target[REGISTERMAXLEN], arg1[REGISTERMAXLEN], arg2[REGISTERMAXLEN];
	if(strcmp(root->childs->name, "INT") == 0){
		reset(op, target, arg1, arg2);
		sprintf(target, "t%d", (*registerNum)++);
		sprintf(arg1, "#%d", root->childs->val.ival);
		newIRcode("=", target, arg1, arg2, ctable);
		// printf("t%d = #%d\n", (*registerNum)++, root->childs->val.ival);
		return (*registerNum)-1;
	}
	else if(strcmp(root->childs->name, "ID") == 0 && root->childs->next == NULL){
		return getSymbolIndex(root->childs->val.sval, stable);
	}
	else if(strcmp(root->childs->next->name, "ASSIGNOP") == 0){
		// 左值
		root->childs->isVisited = 1;
		int ans = translateExp(root->childs->next->next, stable, ctable, registerNum);
		int vnum = getSymbolIndex(root->childs->childs->val.sval, stable);

		reset(op, target, arg1, arg2);
		sprintf(target, "t%d", vnum);
		sprintf(arg1, "t%d", ans);
		newIRcode("=", target, arg1, arg2, ctable);

		// printf("t%d := t%d\n", vnum, ans);
		return -1*vnum;
	}
	else if(strcmp(root->childs->next->name, "PLUS") == 0){
		root->childs->isVisited = 1;
		char c1, c2;
		int num1 = translateExp(root->childs, stable, ctable, registerNum);
		int num2 = translateExp(root->childs->next->next, stable, ctable, registerNum);

		reset(op, target, arg1, arg2);
		sprintf(target, "t%d", (*registerNum)++);
		sprintf(arg1, "t%d", num1);
		sprintf(arg2, "t%d", num2);
		newIRcode("+", target, arg1, arg2, ctable);

		// printf("t%d := t%d + t%d\n", (*registerNum)++, num1, num2);
		return (*registerNum)-1;
	}
	else if(strcmp(root->childs->name, "ID") == 0 && strcmp(root->childs->next->name, "LP") == 0){
		if(strcmp(root->childs->next->next->name, "RP") == 0){
			if(strcmp(root->childs->val.sval, "read") == 0){
				reset(op, target, arg1, arg2);
				sprintf(target, "t%d", (*registerNum)++);
				newIRcode("READ", target, arg1, arg2, ctable);
				// printf("READ t%d\n", (*registerNum)++);
				return (*registerNum)-1;
			}
			else{
				// 无参数函数调用
				reset(op, target, arg1, arg2);
				sprintf(target, "t%d", (*registerNum)++);
				newIRcode(root->childs->val.sval, target, arg1, arg2, ctable);

				// printf("t%d := CALL %s\n", (*registerNum)++, root->childs->val.sval);
				return (*registerNum)-1;	
			}
		}
		else if(strcmp(root->childs->next->next->name, "Args") == 0){
			// 有一个参数
			int num = translateExp(root->childs->next->next->childs, stable, ctable, registerNum);
			if(strcmp(root->childs->val.sval, "write") == 0){
				reset(op, target, arg1, arg2);
				sprintf(arg1, "t%d", num);
				newIRcode("WRITE", target, arg1, arg2, ctable);
				// printf("WRITE %c%d\n", c, num);
			}
		}
		
	}
}

int translateStmt(node* root, symbol_table* stable, code_table* ctable, int* registerNum){
	// Stmt : Exp SEMI
	// 			| CompSt
	// 			| RETURN Exp SEMI
	// 			| IF LP Exp RP Stmt
	// 			| IF LP Exp RP Stmt ELSE Stmt
	root->isVisited = 1;
	char op[10], target[REGISTERMAXLEN], arg1[REGISTERMAXLEN], arg2[REGISTERMAXLEN];
	if(strcmp(root->childs->next->name, "SEMI") == 0){
		return translateExp(root->childs, stable, ctable, registerNum);
	}
	else if(strcmp(root->childs->name, "RETURN") == 0){
		int num1 = translateExp(root->childs->next, stable, ctable, registerNum);
		reset(op, target, arg1, arg2);
		sprintf(target, "t%d", num1);
		newIRcode("RETURN", target, arg1, arg2, ctable);
		// printf("RETURN %c%d\n",c ,num1);
	}
}

void translateFunDec(node* root, symbol_table* stable, code_table* ctable, int* registerNum){
	// FunDec : ID LP VarList RP
	// 			ID LP RP
	root->isVisited = 1;
	char op[10], target[REGISTERMAXLEN], arg1[REGISTERMAXLEN], arg2[REGISTERMAXLEN];

	reset(op, target, arg1, arg2);
	newIRcode("FUNCTION", target, root->childs->val.sval, arg2, ctable);

	// printf("FUNCTION %s :\n", root->childs->val.sval);
}

void generateInterCode(node* root, symbol_table* stable, code_table* ctable, int* registerNum){
	if(root == NULL || root->name == NULL)
		return ;
	
	if(root->isVisited == 0){
		if(strcmp(root->name, "Exp") == 0){
			translateExp(root, stable, ctable, registerNum);
		}
		else if(strcmp(root->name, "Stmt") == 0){
			translateStmt(root, stable, ctable, registerNum);
		}
		else if(strcmp(root->name, "FunDec") == 0){
			translateFunDec(root, stable, ctable, registerNum);
		}
	}
	
	// 遍历整棵树
	if(root->childs != NULL){
		node* sibling = root->childs;
		while(sibling != NULL){
			generateInterCode(sibling, stable, ctable, registerNum);
			sibling = sibling->next;
		}
	}
}