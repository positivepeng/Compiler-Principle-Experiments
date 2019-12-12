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

void printOutInterCode(code_table* ct){
	for(int i = 0;i < ct->totalCnt; i++){
		if(strcmp(ct->codes[i].op, "=") == 0)
			printf("%s := %s\n", ct->codes[i].target, ct->codes[i].arg1);
		else if(strcmp(ct->codes[i].op, "FUNCTION") == 0)
			printf("FUNCTION %s :\n", ct->codes[i].arg1);
		else if(strcmp(ct->codes[i].op, "RETURN") == 0)
			printf("RETURN %s\n", ct->codes[i].arg1);
		else if(strcmp(ct->codes[i].op, "+") == 0 || 
				strcmp(ct->codes[i].op, "-") == 0 || 
				strcmp(ct->codes[i].op, "*") == 0 || 
				strcmp(ct->codes[i].op, "/") == 0){
			printf("%s := %s %s %s\n", ct->codes[i].target, ct->codes[i].arg1, ct->codes[i].op, ct->codes[i].arg2);
		}
		else if(strcmp(ct->codes[i].op, "READ") == 0)
			printf("READ %s\n", ct->codes[i].target);
		else if(strcmp(ct->codes[i].op, "WRITE") == 0)
			printf("WRITE %s\n", ct->codes[i].arg1);
		else if(strcmp(ct->codes[i].op, "CALL") == 0){
			printf("%s := CALL %s\n", ct->codes[i].target, ct->codes[i].arg1);
		}
		else if(strcmp(ct->codes[i].op, "IF") == 0){
			printf("IF %s %s %s ",ct->codes[i].arg1, ct->codes[i].target, ct->codes[i].arg2);
		}
		else if(strcmp(ct->codes[i].op, "GOTO") == 0){
			printf("GOTO %s\n", ct->codes[i].arg1);
		}
		else if(strcmp(ct->codes[i].op, "LABEL") == 0)
			printf("LABEL %s : \n", ct->codes[i].arg1);
		else if(strcmp(ct->codes[i].op, "ARG") == 0)
			printf("ARG %s\n", ct->codes[i].arg1);
		else if(strcmp(ct->codes[i].op, "PARAM") == 0)
			printf("PARAM %s\n", ct->codes[i].arg1);
	}
}

void addLabelIR(int label, code_table* ctable){
	char op[10], target[REGISTERMAXLEN], arg1[REGISTERMAXLEN], arg2[REGISTERMAXLEN];
	reset(op, target, arg1, arg2);
	sprintf(arg1, "label%d", label);
	newIRcode("LABEL", target, arg1, arg2, ctable);
}

void addGotoIR(int label, code_table* ctable){
	char op[10], target[REGISTERMAXLEN], arg1[REGISTERMAXLEN], arg2[REGISTERMAXLEN];
	reset(op, target, arg1, arg2);
	sprintf(arg1, "label%d", label);
	newIRcode("GOTO", target, arg1, arg2, ctable);
}

void translateCond(node* exp, int labelTrue, int labelFalse, symbol_table* stable, code_table* ctable, int* registerNum, int* labelNum){
	// 标记已访问
	exp->isVisited = 1;

	char op[10], target[REGISTERMAXLEN], arg1[REGISTERMAXLEN], arg2[REGISTERMAXLEN];

	if(strcmp(exp->childs->name, "NOT") == 0){
		// Exp : Not Exp1
		translateCond(exp, labelFalse, labelTrue, stable, ctable, registerNum, labelNum);
	}
	else if(strcmp(exp->childs->next->name, "RELOP") == 0){
		// Exp : Exp1 RELOP Exp2
		
		char* relop = exp->childs->next->val.sval;
		node* exp1 = exp->childs, *exp2 = exp->childs->next->next;
		int t1 = translateExp(exp1, stable, ctable, registerNum, labelNum);
		int t2 = translateExp(exp2, stable, ctable, registerNum, labelNum);

		// printf("find %s: %d %s: %d \n", exp1->childs->val.sval, t1, exp2->childs->val.sval, t2);
		// code3 = if t1 op t2 goto label_true
		reset(op, target, arg1, arg2);
		sprintf(target, "%s", relop);
		sprintf(arg1, "t%d", t1);
		sprintf(arg2, "t%d", t2);
		newIRcode("IF", target, arg1, arg2, ctable);
		
		// code3 goto label_true
		addGotoIR(labelTrue, ctable);

		// code3 goto label_false
		addGotoIR(labelFalse, ctable);
	}
	else if(strcmp(exp->childs->next->name, "AND") == 0){
		// EXP : Exp1 AND Exp2
		int label1 = (*labelNum)++;
		translateCond(exp->childs, label1, labelFalse, stable, ctable, registerNum, labelNum);
		addLabelIR(label1, ctable);
		translateCond(exp->childs->next->next, labelTrue, labelFalse, stable, ctable, registerNum, labelNum);
	}
}

int translateExp(node* root, symbol_table* stable, code_table* ctable, int* registerNum, int* labelNum){
	// 返回结果存入的寄存器的编号
	// 解析Exp

	if(root == NULL)
		printf("yes root may be null\n");
	if(root->childs == NULL)
		printf("yes root's child may be null\n");

	// 标记该节点已访问
	root->isVisited = 1;

	char op[10], target[REGISTERMAXLEN], arg1[REGISTERMAXLEN], arg2[REGISTERMAXLEN];
	if((root->childs != NULL && strcmp(root->childs->name, "NOT") == 0) || 
	 	(root->childs->next != NULL && (strcmp(root->childs->next->name, "RELOP") == 0 ||
		strcmp(root->childs->next->name, "AND") == 0 || 
		strcmp(root->childs->next->name, "OR") == 0 ))){
		//Exp:		Exp RELOP Exp
		// 			NOT Exp
		// 			Exp1 AND Exp2
		// 			Exp1 OR Exp2
		int labelTrue = (*labelNum)++;
		int labelFalse = (*labelNum)++;
		int temp = (*registerNum)++;

		// code0 place := #0
		reset(op, target, arg1, arg2);
		sprintf(target, "t%d", temp);
		sprintf(arg1, "#0");
		newIRcode("=", target, arg1, arg2, ctable);

		// code1
		translateCond(root, labelTrue, labelFalse, stable, ctable, registerNum, labelNum);

		// code2 LABEL label1 
		addLabelIR(labelTrue, ctable);

		// place := 1
		reset(op, target, arg1, arg2);
		sprintf(target, "t%d", temp);
		sprintf(arg1, "#1");
		newIRcode("=", target, arg1, arg2, ctable);

		// LABEL label2
		addLabelIR(labelFalse, ctable);

		return temp;
	}
	else if(root->childs != NULL && strcmp(root->childs->name, "INT") == 0){
		// 			INT
		reset(op, target, arg1, arg2);
		sprintf(target, "t%d", (*registerNum)++);
		sprintf(arg1, "#%d", root->childs->val.ival);
		newIRcode("=", target, arg1, arg2, ctable);
		return (*registerNum)-1;
	}
	else if(root->childs != NULL && root->childs->next == NULL && strcmp(root->childs->name, "ID") == 0){
		//			ID
		return getSymbolIndex(root->childs->val.sval, stable);
	}
	else if(root->childs->next != NULL && strcmp(root->childs->next->name, "ASSIGNOP") == 0){
		// Exp : 	Exp ASSIGNOP Exp
	
		// 左EXP标记已访问
		root->childs->isVisited = 1;

		//　获取ID所在的寄存器编号，翻译右EXP
		int vnum = getSymbolIndex(root->childs->childs->val.sval, stable);
		int ans = translateExp(root->childs->next->next, stable, ctable, registerNum, labelNum);
		
		reset(op, target, arg1, arg2);
		sprintf(target, "t%d", vnum);
		sprintf(arg1, "t%d", ans);
		newIRcode("=", target, arg1, arg2, ctable);

		return vnum;
	}
	else if(root->childs->next != NULL && (strcmp(root->childs->next->name, "PLUS") == 0 || 
			strcmp(root->childs->next->name, "MINUS") == 0 || 
			strcmp(root->childs->next->name, "STAR") == 0 || 
			strcmp(root->childs->next->name, "DIV") == 0)){
		// 			Exp PLUS Exp
		// 			Exp MINUS Exp
		// 			Exp STAR Exp
		// 			Exp DIV Exp
		// 左exp标记已访问
		root->childs->isVisited = 1;

		// 翻译子exp
		int num1 = translateExp(root->childs, stable, ctable, registerNum, labelNum);
		int num2 = translateExp(root->childs->next->next, stable, ctable, registerNum, labelNum);

		// 生成中间代码
		reset(op, target, arg1, arg2);
		sprintf(target, "t%d", (*registerNum)++);
		sprintf(arg1, "t%d", num1);
		sprintf(arg2, "t%d", num2);
		if(strcmp(root->childs->next->name, "PLUS") == 0)
			newIRcode("+", target, arg1, arg2, ctable);
		if(strcmp(root->childs->next->name, "MINUS") == 0)
			newIRcode("-", target, arg1, arg2, ctable);
		if(strcmp(root->childs->next->name, "STAR") == 0)
			newIRcode("*", target, arg1, arg2, ctable);
		if(strcmp(root->childs->next->name, "DIV") == 0)
			newIRcode("/", target, arg1, arg2, ctable);

		return (*registerNum)-1;
	}
	else if(root->childs != NULL && root->childs->next != NULL && strcmp(root->childs->name, "ID") == 0 && strcmp(root->childs->next->name, "LP") == 0){
		// 			ID LP Args RP
		// 			ID LP RP
		
		if(strcmp(root->childs->next->next->name, "RP") == 0){
			if(strcmp(root->childs->val.sval, "read") == 0){
				// 调用系统函数read
				reset(op, target, arg1, arg2);
				sprintf(target, "t%d", (*registerNum)++);
				newIRcode("READ", target, arg1, arg2, ctable);
				return (*registerNum)-1;
			}
			else{
				// 无参数函数调用
				// 未做测试
				reset(op, target, arg1, arg2);
				sprintf(target, "t%d", (*registerNum)++);
				sprintf(arg1, "%s", root->childs->val.sval);
				newIRcode("CALL", target, arg1, arg2, ctable);
				return (*registerNum)-1;	
			}
		}
		else if(strcmp(root->childs->next->next->name, "Args") == 0){
			// 有一个参数
			int num = translateExp(root->childs->next->next->childs, stable, ctable, registerNum, labelNum);
			if(strcmp(root->childs->val.sval, "write") == 0){
				// 调用write
				reset(op, target, arg1, arg2);
				sprintf(arg1, "t%d", num);
				newIRcode("WRITE", target, arg1, arg2, ctable);
			}
			else{   // 自定义的一个函数的参数
				// Args : Exp 
				node* exp = root->childs->next->next->childs;
				int num1 = translateExp(exp, stable, ctable, registerNum, labelNum);
				reset(op, target, arg1, arg2);
				sprintf(arg1, "t%d", num1);
				newIRcode("ARG", target, arg1, arg2, ctable);

				reset(op, target, arg1, arg2);
				sprintf(target, "t%d", (*registerNum)++);
				sprintf(arg1, "%s", root->childs->val.sval);
				newIRcode("CALL", target, arg1, arg2, ctable);
				return (*registerNum)-1;	
			}
		}	
	}
}

void translateStmt(node* stmt, symbol_table* stable, code_table* ctable, int* registerNum, int* labelNum){
	// 标记该节点已被访问
	if(stmt == NULL)
		return ;

	stmt->isVisited = 1;

	char op[10], target[REGISTERMAXLEN], arg1[REGISTERMAXLEN], arg2[REGISTERMAXLEN];
	
	if(stmt->childs != NULL && strcmp(stmt->childs->name, "CompSt") == 0){
		// Stmt  : CompSt
		// CompSt : LC DefList StmtList RC
		// 假设这里的DefList为空
		node* stmtList = stmt->childs->childs->next->next;
		translateStmtList(stmtList, stable, ctable, registerNum, labelNum);
	}
	else if(strcmp(stmt->childs->next->name, "SEMI") == 0){
		// Stmt : Exp SEMI
		translateExp(stmt->childs, stable, ctable, registerNum, labelNum);
	}
	else if(strcmp(stmt->childs->name, "RETURN") == 0){
		// Stmt : RETURN Exp SEMI
		node* exp = stmt->childs->next;
		int num1 = translateExp(exp, stable, ctable, registerNum, labelNum);

		reset(op, target, arg1, arg2);
		sprintf(arg1, "t%d", num1);
		newIRcode("RETURN", target, arg1, arg2, ctable);
	}
	else if(strcmp(stmt->childs->name, "IF") == 0 && stmt->childs->next->next->next->next->next == NULL){
		//		: IF LP Exp RP Stmt
		int labelTrue = (*labelNum)++;
		int labelFalse = (*labelNum)++;
		node* exp = stmt->childs->next->next;
		// exp未调用translate函数所以需要在这里标记已访问
		exp->isVisited = 1;

		translateCond(exp, labelTrue, labelFalse, stable, ctable, registerNum, labelNum);

		addLabelIR(labelTrue, ctable);
		translateStmt(stmt->childs->next->next->next->next, stable, ctable, registerNum, labelNum);
		addLabelIR(labelFalse, ctable);
	}
	else if(strcmp(stmt->childs->name, "IF") == 0 && strcmp(stmt->childs->next->next->next->next->next->name, "ELSE") == 0){
		// 		: IF LP Exp RP Stmt ELSE Stmt
		int label1 = (*labelNum)++;
		int label2 = (*labelNum)++;
		int label3 = (*labelNum)++;
		translateCond(stmt->childs->next->next, label1, label2, stable, ctable, registerNum, labelNum);
		addLabelIR(label1, ctable);
		translateStmt(stmt->childs->next->next->next->next, stable, ctable, registerNum, labelNum);
		addGotoIR(label3, ctable);
		addLabelIR(label2, ctable);
		translateStmt(stmt->childs->next->next->next->next->next->next, stable, ctable, registerNum, labelNum);
		addLabelIR(label3, ctable);
	}
	else if(strcmp(stmt->childs->name, "WHILE") == 0){
		// WHILE LP EXP RP Stmt
		int label1 = (*labelNum)++;
		int label2 = (*labelNum)++;
		int label3 = (*labelNum)++;
		node* exp = stmt->childs->next->next;

		addLabelIR(label1, ctable);
		translateCond(exp, label2, label3, stable, ctable, registerNum, labelNum);
		addLabelIR(label2, ctable);			
		translateStmt(stmt->childs->next->next->next->next, stable, ctable, registerNum, labelNum);
		addGotoIR(label1, ctable);
		addLabelIR(label3, ctable);
	}
}

void translateStmtList(node* stmtList, symbol_table* stable, code_table* ctable, int* registerNum, int* labelNum){
	//StmtList : Stmt StmtList
	// : empty
	if(stmtList == NULL || stmtList->childs == NULL)
		return ;
	node* nextStmtList = stmtList->childs->next;
	translateStmt(stmtList->childs, stable, ctable, registerNum, labelNum);
	translateStmtList(nextStmtList, stable, ctable, registerNum, labelNum);
}

void translateFunDec(node* root, symbol_table* stable, code_table* ctable, int* registerNum){
	// FunDec : ID LP VarList RP
	// 			ID LP RP
	
	// 标记该节点已被访问
	root->isVisited = 1;

	// 添加一行中间代码，表面函数开头
	char op[10], target[REGISTERMAXLEN], arg1[REGISTERMAXLEN], arg2[REGISTERMAXLEN];
	reset(op, target, arg1, arg2);
	newIRcode("FUNCTION", target, root->childs->val.sval, arg2, ctable);

	// 函数定义如果有参数[假定只有一个参数]
	if(strcmp(root->childs->next->next->name, "VarList") == 0){
		// VarList : ParamDec
		// ParamDec : Specifier VarDec
		// VarDec : ID
		reset(op, target, arg1, arg2);
		node* varList = root->childs->next->next;
		sprintf(arg1, "t%d", getSymbolIndex(varList->childs->childs->next->childs->val.sval, stable));
		newIRcode("PARAM", target, arg1, arg2, ctable);
	}
}

void generateInterCode(node* root, symbol_table* stable, code_table* ctable, int* registerNum, int* labelNum){
	if(root == NULL || root->name == NULL)
		return ;

	// 检查节点是否已被访问过
	if(root->isVisited == 0){
		if(strcmp(root->name, "Exp") == 0){
			translateExp(root, stable, ctable, registerNum, labelNum);
		}
		else if(strcmp(root->name, "Stmt") == 0){
			translateStmt(root, stable, ctable, registerNum, labelNum);
		}
		else if(strcmp(root->name, "FunDec") == 0){
			translateFunDec(root, stable, ctable, registerNum);
		}
	}
	
	// 遍历整棵树
	if(root->childs != NULL){
		node* sibling = root->childs;
		while(sibling != NULL){
			generateInterCode(sibling, stable, ctable, registerNum, labelNum);
			sibling = sibling->next;
		}
	}
}