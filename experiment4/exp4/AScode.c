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

void generateAssemblyCode(code_table* ct){
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
		else if(strcmp(ct->codes[i].op, "=") == 0 && ct->codes[i].arg1[0] == 't'){
			int tar = getRegister(ct->codes[i].target, memory, regs, used) - base;
			int arg = getRegister(ct->codes[i].arg1, memory, regs, used) - base;
			printf("\tmove $t%d, $t%d\n", tar, arg);
		}
		else if(strcmp(ct->codes[i].op, "=") == 0 && ct->codes[i].arg1[0] == '#'){
			int tar = getRegister(ct->codes[i].target, memory, regs, used) - base;
			printf("\taddi $t%d %d\n", tar, atoi(ct->codes[i].arg1+1));
		}
		else if(strcmp(ct->codes[i].op, "+") == 0){
			int tar = getRegister(ct->codes[i].target, memory, regs, used) - base;
			int a1 = getRegister(ct->codes[i].arg1, memory, regs, used) - base;
			int a2 = getRegister(ct->codes[i].arg2, memory, regs, used) - base;
			printf("\tadd $t%d, $t%d, $t%d\n", tar, a1, a2);
		}
		else if(strcmp(ct->codes[i].op, "WRITE") == 0){
			printf("\tmove $a0, $t%d\n", getRegister(ct->codes[i].arg1, memory, regs, used)-base);
			printf("\taddi $sp, $sp, -4\n");
			printf("\tsw $ra, 0($sp)\n");
			printf("\tjal write\n");
			printf("\tlw $ra, 0($sp)\n");
			printf("\taddi $sp, $sp, 4\n");
		}
		else if(strcmp(ct->codes[i].op, "RETURN") == 0){
			printf("\tmove $v0, $t%d\n", getRegister(ct->codes[i].target, memory, regs, used)-base);
			printf("\tjr $ra\n");
		}
	}
}