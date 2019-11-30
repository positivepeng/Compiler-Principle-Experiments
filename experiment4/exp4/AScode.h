#include "IRcode.h"

void printHead();
int getRegister(char* variableName, int* memory, int regs[32], int used[32]);
void generateAssemblyCode(code_table* ct);