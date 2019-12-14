#include "AScode.h"

void printHead(){
	printf(".data\n");
	printf("_prompt: .asciiz \"Enter an integer:\"\n");
	printf("_ret: .asciiz \"\\n\"\n");
	printf(".globl main\n");
	printf(".text\n");
	printf("read:\n");
	printf("\tli $v0, 4\n");
	printf("\tla $a0, _prompt\n");
	printf("\tsyscall\n");
	printf("\tli $v0, 5\n");
	printf("\tsyscall\n");
	printf("\tjr $ra\n");

	printf("\n");

	printf("write:\n");
    printf("\tli $v0, 1\n");
    printf("\tsyscall\n");
	printf("\tli $v0, 4\n");
	printf("\tla $a0, _ret\n");
	printf("\tsyscall\n");
	printf("\tmove $v0, $0\n");
	printf("\tjr $ra\n");
}

int getRegister(char* variableName, int* memory, int regs[32], int used[32]){
	// 分配寄存器 分配t0-t9
	// 每个variableName(形式为t#)都对应一个memory中的位置
	int tnum = atoi(variableName+1);

	// 假设variableName对应memory[tnum]
	// printf("%s->%d\n", variableName, tnum);
	int index = -1; // 空闲寄存器的编号
	int base = 8; 	// $t0 == $8
	for(int i = 0;i <= 9; i++){
		int k = base + i; 		// 寄存器编号
		if(used[k] == tnum)		// 第k个寄存器存的就是tnum
			return k;
		else if(used[k] == 0 && index == -1)
			index = k;

	}
	if(index == -1){     // 没有空余的,把第一个可写的寄存器的值存入内存
		memory[used[base]] = regs[base];
		used[base] = tnum;
		return base;
	}
	else{
		used[index] = tnum;   // 分配空闲的给它
		return index;
	}
}

// void printOutInterCode(code_table* ct){
// 	for(int i = 0;i < ct->totalCnt; i++)
// 		else if(strcmp(ct->codes[i].op, "CALL") == 0){
// 			printf("%s := CALL %s\n", ct->codes[i].target, ct->codes[i].arg1);
// 		}
// 		else if(strcmp(ct->codes[i].op, "IF") == 0){
// 			printf("IF %s %s %s ",ct->codes[i].arg1, ct->codes[i].target, ct->codes[i].arg2);
// 		}
// 		else if(strcmp(ct->codes[i].op, "GOTO") == 0){
// 			printf("GOTO %s\n", ct->codes[i].arg1);
// 		}
// 		else if(strcmp(ct->codes[i].op, "LABEL") == 0)
// 			printf("LABEL %s : \n", ct->codes[i].arg1);
// 		else if(strcmp(ct->codes[i].op, "ARG") == 0)
// 			printf("ARG %s\n", ct->codes[i].arg1);
// 		else if(strcmp(ct->codes[i].op, "PARAM") == 0)
// 			printf("PARAM %s\n", ct->codes[i].arg1);
// 	}
// }

void generateAssemblyCode(code_table* ct, symbol_table* st){
	int regs[32], used[32], regNum, memory[100], base = 8;
	// init
	printHead();
	memset(regs, 0, sizeof(regs));
	memset(used, 0, sizeof(used));

	for(int i = 0;i < ct->totalCnt; i++){
		if(strcmp(ct->codes[i].op, "FUNCTION") == 0){
			printf("%s:\n", ct->codes[i].arg1);
		}
		else if(strcmp(ct->codes[i].op, "READ") == 0){
			printf("\taddi $sp, $sp, -4\n");
			printf("\tsw $ra, 0($sp)\n");
			printf("\tjal read\n");
			printf("\tlw $ra, 0($sp)\n");
			printf("\taddi $sp, $sp, 4\n");
			printf("\tmove $t%d, $v0\n", getRegister(ct->codes[i].target, memory, regs, used)-base);
		}
		else if(strcmp(ct->codes[i].op, "WRITE") == 0){
			printf("\tmove $a0, $t%d\n", getRegister(ct->codes[i].arg1, memory, regs, used)-base);
			printf("\taddi $sp, $sp, -4\n");
			printf("\tsw $ra, 0($sp)\n");
			printf("\tjal write\n");
			printf("\tlw $ra, 0($sp)\n");
			printf("\taddi $sp, $sp, 4\n");
		}
		else if(strcmp(ct->codes[i].op, "=") == 0 && ct->codes[i].arg1[0] == 't'){
			int tar = getRegister(ct->codes[i].target, memory, regs, used) - base;
			int arg = getRegister(ct->codes[i].arg1, memory, regs, used) - base;
			printf("\tmove $t%d, $t%d\n", tar, arg);
		}
		else if(strcmp(ct->codes[i].op, "=") == 0 && ct->codes[i].arg1[0] == '#'){
			int tar = getRegister(ct->codes[i].target, memory, regs, used) - base;
			printf("\tli $t%d %d\n", tar, atoi(ct->codes[i].arg1+1));  // +1表示去除符号
		}
		else if((strcmp(ct->codes[i].op, "+") == 0) || (strcmp(ct->codes[i].op, "-") == 0) || 
			(strcmp(ct->codes[i].op, "*") == 0)){
			int tar = getRegister(ct->codes[i].target, memory, regs, used) - base;
			int a1 = getRegister(ct->codes[i].arg1, memory, regs, used) - base;
			int a2 = getRegister(ct->codes[i].arg2, memory, regs, used) - base;
			if(strcmp(ct->codes[i].op, "+") == 0)
				printf("\tadd $t%d, $t%d, $t%d\n", tar, a1, a2);
			else if(strcmp(ct->codes[i].op, "-") == 0)
				printf("\tsub $t%d, $t%d, $t%d\n", tar, a1, a2);
			else if(strcmp(ct->codes[i].op, "*") == 0)
				printf("\tmul $t%d, $t%d, $t%d\n", tar, a1, a2);
		}
		else if(strcmp(ct->codes[i].op, "RETURN") == 0){
			printf("\tmove $v0, $t%d\n", getRegister(ct->codes[i].arg1, memory, regs, used)-base);
			printf("\tjr $ra\n");
		}
		// else if(strcmp(ct->codes[i].op, "CALL") == 0){
		// 	printf("\taddi $sp, $sp, -8\n");
		// 	printf("\tsw $ra, 0($sp)\n");
		// 	printf("\tjal %s\n", ct->codes[i].arg1);
		// 	//printf("\tmove $t%d $v0\n", getRegister(ct->codes[i].target, memory, regs, used)-base);
		// 	printf("\tlw $ra, 0($sp)\n");
		// 	printf("\taddi $sp, $sp, 4\n");
		// }
		else if(strcmp(ct->codes[i].op, "IF") == 0){
			int r1 = getRegister(ct->codes[i].arg1, memory, regs, used)-base;
			int r2 = getRegister(ct->codes[i].arg2, memory, regs, used)-base;
			
			char* label = ct->codes[i+1].arg1;

			if(strcmp(ct->codes[i].target, "==") == 0)
				printf("\tbeq $t%d, $t%d, %s\n", r1, r2, label);
			else if(strcmp(ct->codes[i].target, "!=") == 0)
				printf("\tbne $t%d, $t%d, %s\n", r1, r2, label);
			else if(strcmp(ct->codes[i].target, ">") == 0)
				printf("\tbgt $t%d, $t%d, %s\n", r1, r2, label);
			else if(strcmp(ct->codes[i].target, "<") == 0)
				printf("\tblt $t%d, $t%d, %s\n", r1, r2, label);
			else if(strcmp(ct->codes[i].target, ">=") == 0)
				printf("\tbge $t%d, $t%d, %s\n", r1, r2, label);
			else if(strcmp(ct->codes[i].target, "<=") == 0)
				printf("\tble $t%d, $t%d, %s\n", r1, r2, label);
			i++;
		}
		else if(strcmp(ct->codes[i].op, "GOTO") == 0){
			printf("\tj %s\n", ct->codes[i].arg1);
		}
		else if(strcmp(ct->codes[i].op, "LABEL") == 0){
			printf("%s:\n", ct->codes[i].arg1);
		}
		else if(strcmp(ct->codes[i].op, "ARG") == 0){
			// 通过栈传递参数
			// 下一条必定是CALL
			// 存返回地址
			printf("\taddi $sp, $sp, -4\n");
			printf("\tsw $ra, 0($sp)\n");
			// 保存局部变量
			char* funcName = ct->codes[i+1].arg1;
			symbol* sym = findSymbolInTable(funcName, st);
			if(sym->cnt > 0){
				char* paramName = (sym->fieldName)[0];
				int paramIndex = getSymbolIndex(paramName, st);
				// 把参数赋值给对应的函数参数寄存器
				char param[10];
				sprintf(param, "t%d", paramIndex);
				int paramR = getRegister(param, memory, regs, used)-base;	
				printf("\taddi $sp, $sp, -4\n");
				printf("\tsw $t%d, 0($sp)\n", paramR);
			}
			
			// 存传入函数的参数
			printf("\taddi $sp, $sp, -4\n");
			printf("\tsw $t%d, 0($sp)\n", getRegister(ct->codes[i].arg1, memory, regs, used)-base);
			// 调用函数
			printf("\tjal %s\n", ct->codes[i+1].arg1);
			if(sym->cnt > 0){
				// 恢复局部变量
				char* paramName = (sym->fieldName)[0];
				int paramIndex = getSymbolIndex(paramName, st);
				// 把参数赋值给对应的函数参数寄存器
				char param[10];
				sprintf(param, "t%d", paramIndex);
				int paramR = getRegister(param, memory, regs, used)-base;
				printf("\tlw $t%d, 0($sp)\n", paramR);
				printf("\taddi $sp, $sp, 4\n");	
			}
			
			// 恢复返回地址
			printf("\tlw $ra, 0($sp)\n");
			printf("\taddi $sp, $sp, 4\n");
			
			// 得到函数调用结果
			printf("\tmove $t%d, $v0\n", getRegister(ct->codes[i+1].target, memory, regs, used)-base);
			
			// int valueR = getRegister(ct->codes[i].arg1, memory, regs, used)-base;  // 分别为参数对应的寄存器的编号和传进函数的值对应的编号
			// printf("$t%d = $t%d\n", paramR, valueR);
		}
		else if(strcmp(ct->codes[i].op, "PARAM") == 0){
			// 函数形参数，通过栈传递参数
			printf("\tlw $t%d, 0($sp)\n", getRegister(ct->codes[i].arg1, memory, regs, used)-base);
			printf("\taddi $sp, $sp, 4\n");
		}
	}
}