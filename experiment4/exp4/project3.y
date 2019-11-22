%locations
%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "symbol.h"
	extern int yylex();
	extern int yyparse();
	extern FILE* yyin;
	extern int yylineno;

	node* root;
	int debug = 0;
	int haserror = 0;
	symbol_table symTable;

	void yyerror(const char* s);
%}

%union {
	struct NODE* npval;
}

%token<npval> INT
%token<npval> FLOAT
%token<npval> ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV AND OR DOT NOT TYPE
%token<npval> LP RP LB RB LC RC
%token<npval> STRUCT RETURN IF ELSE WHILE

// 非终结符
%type<npval> Program ExtDefList ExtDef ExtDecList Specifier FunDec CompSt VarDec
%type<npval> StructSpecifier OptTag DefList Tag
%type<npval> VarList ParamDec
%type<npval> StmtList Stmt Def Dec DecList Exp Args


// 开始符
%start Program

// 优先级定义
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%nonassoc LOWER_THAN_COMMA
%nonassoc COMMA

%left ASSIGNOP
%left AND OR
%left RELOP
%left PLUS MINUS 
%left STAR DIV
%right NOT
%left LC RC LP RP


%%

Program : ExtDefList {
	$$ = newNode(-1, "Program", NULL);
	addChild(2, $$, $1);
	root = $$;
	if(debug)
		printf("parse : %s -> %s\n", $$->name, $1->name);
}
;
ExtDefList : ExtDef ExtDefList{
	$$ = (node*)newNode(-1, "ExtDefList", NULL);
	addChild(3, $$, $1, $2);
	if(debug)
		printf("parse : %s -> %s %s\n", $$->name, $$->childs->name, $1->next == NULL ? "" : $1->next->name);
}
		| {
	$$ = (node*)newNode(EMPTYNODETOKEN, "ExtDefList", NULL);
}
;
ExtDef : Specifier ExtDecList SEMI{
	$$ = (node*) newNode(-1, "ExtDef", NULL);
	addChild(4, $$, $1, $2, $3);
}
		| Specifier SEMI{
	$$ = (node*)newNode(-1, "ExtDef", NULL);
	addChild(3, $$, $1, $2);
	if(debug)
		printf("parse : %s -> %s %s\n", $$->name, $$->childs->name, $1->next->name);
}
		| Specifier FunDec CompSt{
	$$ = (node*)newNode(-1, "ExtDef", NULL);
	addChild(4, $$, $1, $2, $3);
}
		| Specifier{
	$$ = (node*)newNode(-1, "ExtDef", NULL);
	node* newSEMI = (node*)newNode(SEMI, "SEMI", NULL);
	addChild(3, $$, $1, newSEMI);
	printf("Recovering From Error: Expected \';\' .\n");
	if(debug)
		printf("parse : %s -> %s %s\n", $$->name, $$->childs->name, $1->next->name);
}
;
ExtDecList : VarDec{
	$$ = (node*) newNode(-1, "ExtDecList", NULL);
	addChild(2, $$, $1);
}
		| VarDec COMMA ExtDecList{
	$$ = (node*) newNode(-1, "ExtDecList", NULL);
	addChild(4, $$, $1, $2, $3);
}
;

Specifier : TYPE{
	$$ = (node*)newNode(-1, "Specifier", NULL);
	addChild(2, $$, $1);
	if(debug)
		printf("parse : %s -> %s\n", $$->name, $$->childs->name);
}
		| StructSpecifier{
	$$ = (node*)newNode(-1, "Specifier", NULL);
	addChild(2, $$, $1);	
	if(debug)
		printf("parse : %s -> %s\n", $$->name, $$->childs->name);
}
;
StructSpecifier : STRUCT OptTag LC DefList RC{
	$$ = (node*)newNode(-1, "StructSpecifier", NULL);
	addChild(6, $$, $1, $2, $3, $4, $5);
	if(debug)
		printf("parse : %s -> %s %s %s %s %s\n", $$->name, $1->name, $1->next->name, $2->next->name, $3->next->name, $4->next->name);	
}
		| STRUCT Tag{
	$$ = (node*)newNode(-1, "StructSpecifier", NULL);
	addChild(3, $$, $1, $2);
}
;
OptTag : ID{
	$$ = (node*)newNode(-1, "OptTag", NULL);
	addChild(2, $$, $1);
	if(debug)
		printf("parse : %s -> %s\n", $$->name, $1->name);
}
		|{
	$$ = (node*)newNode(EMPTYNODETOKEN, "OptTag", NULL);
}
;
Tag : ID{
	$$ = (node*)newNode(-1, "Tag", NULL);
	addChild(2, $$, $1);
	if(debug)
		printf("parse : %s -> %s\n", $$->name, $1->name);
}
;


VarDec : ID{
	$$ = (node*)newNode(-1, "VarDec", NULL);
	addChild(2, $$, $1);
	if(debug)
		printf("parse : %s -> %s\n", $$->name, $1->name);
}
		| VarDec LB INT RB{
	$$ = (node*)newNode(-1, "VarDec", NULL);
	addChild(5, $$, $1, $2, $3, $4);
}
;
FunDec : ID LP VarList RP{
	$$ = (node*)newNode(-1, "FunDec", NULL);
	addChild(5, $$, $1, $2, $3, $4);
}
		| ID LP RP{
	$$ = (node*)newNode(-1, "FunDec", NULL);
	addChild(4, $$, $1, $2, $3);
}
;
VarList : ParamDec COMMA VarList{
	$$ = (node*)newNode(-1, "VarList", NULL);
	addChild(4, $$, $1, $2, $3);
}
		| ParamDec{
	$$ = (node*)newNode(-1, "VarList", NULL);
	addChild(2, $$, $1);	
}
;
ParamDec : Specifier VarDec{
	$$ = (node*)newNode(-1, "ParamDec", NULL);
	addChild(3, $$, $1, $2);
}
;


CompSt : LC DefList StmtList RC{
	$$ = (node*)newNode(-1, "CompSt", NULL);
	addChild(5, $$, $1, $2, $3, $4);
}
;
StmtList : Stmt StmtList{
	$$ = (node*)newNode(-1, "StmtList", NULL);
	addChild(3, $$, $1, $2);	
}
		|{
	$$ = (node*)newNode(EMPTYNODETOKEN, "StmtList", NULL);
}
;
Stmt : Exp SEMI{
	$$ = (node*)newNode(-1, "Stmt", NULL);
	addChild(3, $$, $1, $2);
}
		| CompSt{
	$$ = (node*)newNode(-1, "Stmt", NULL);
	addChild(2, $$, $1);	
}
		| RETURN Exp SEMI{
	$$ = (node*)newNode(-1, "Stmt", NULL);
	addChild(4, $$, $1, $2, $3);
}
		| IF LP Exp RP Stmt    %prec LOWER_THAN_ELSE{
	$$ = (node*)newNode(-1, "Stmt", NULL);
	addChild(6, $$, $1, $2, $3, $4, $5);
}
		| IF LP Exp RP Stmt ELSE Stmt{
	$$ = (node*)newNode(-1, "Stmt", NULL);
	addChild(8, $$, $1, $2, $3, $4, $5, $6, $7);
}
		| WHILE LP Exp RP Stmt{
	$$ = (node*)newNode(-1, "Stmt", NULL);
	addChild(6, $$, $1, $2, $3, $4, $5);
}	
		| error SEMI{
	printf("Invalid Statement\n");
	exit(1);
}
;

DefList : Def DefList{
	$$ = (node*)newNode(-1, "DefList", NULL);
	addChild(3, $$, $1, $2);
	if(debug)
		printf("parse : %s -> %s %s\n", $$->name, $1->name, $1->next == NULL ? "" : $1->next->name);
}
		|{
	$$ = (node*)newNode(EMPTYNODETOKEN, "DefList", NULL);
}
;
Def : Specifier DecList SEMI{
	$$ = (node*)newNode(-1, "Def", NULL);
	addChild(4, $$, $1, $2, $3);
	if(debug)
		printf("parse : %s -> %s %s %s\n", $$->name, $1->name, $1->next->name, $2->next->name);
}		| error SEMI{
	printf("Invalid Definition\n");
	exit(1);
}
;
DecList : Dec 	%prec LOWER_THAN_COMMA{
	$$ = (node*)newNode(-1, "DecList", NULL);
	addChild(2, $$, $1);
	if(debug)
		printf("parse : %s -> %s\n", $$->name, $1->name);
}
		| Dec COMMA DecList{
	$$ = (node*)newNode(-1, "DecList", NULL);
	addChild(4, $$, $1, $2, $3);
}
;
Dec : VarDec{
	$$ = (node*)newNode(-1, "Dec", NULL);
	addChild(2, $$, $1);
	if(debug)
		printf("parse : %s -> %s\n", $$->name, $1->name);
}
		| VarDec ASSIGNOP Exp{
	$$ = (node*)newNode(-1, "Dec", NULL);
	addChild(4, $$, $1, $2, $3);
}
;


Exp : Exp ASSIGNOP Exp{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(4, $$, $1, $2, $3);
}
		| Exp AND Exp{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(4, $$, $1, $2, $3);		
}
		| Exp OR Exp{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(4, $$, $1, $2, $3);
}
		| Exp RELOP Exp{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(4, $$, $1, $2, $3);
}
		| Exp PLUS Exp{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(4, $$, $1, $2, $3);
}
		| Exp MINUS Exp{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(4, $$, $1, $2, $3);
}
		| Exp STAR Exp{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(4, $$, $1, $2, $3);
}
		| Exp DIV Exp{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(4, $$, $1, $2, $3);
}
		| LP Exp RP{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(4, $$, $1, $2, $3);
}
		| MINUS Exp{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(3, $$, $1, $2);
}
		| NOT Exp{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(3, $$, $1, $2);
}
		| ID LP Args RP{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(5, $$, $1, $2, $3, $4);
}
		| ID LP RP{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(4, $$, $1, $2, $3);
}
		| Exp LB Exp RB{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(5, $$, $1, $2, $3, $4);
}
		| Exp DOT ID{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(4, $$, $1, $2, $3);
}
		| ID{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(2, $$, $1);
}
		| INT{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(2, $$, $1);
}
		| FLOAT{
	$$ = (node*)newNode(-1, "Exp", NULL);
	addChild(2, $$, $1);
}
;

Args : Exp COMMA Args{
	$$ = (node*)newNode(-1, "Args", NULL);
	addChild(4, $$, $1, $2, $3);
}
		| Exp{
	$$ = (node*)newNode(-1, "Args", NULL);
	addChild(2, $$, $1);	
}
;
%%

int main(int argc, char** argv) {
	
	if(argc > 1){
		if(!(yyin = fopen(argv[1], "r"))){
			perror(argv[1]);
			return 1;
		}
	}

	// 调试
	// yydebug = 0;
	
	// 解析输入的词法单元
	yyparse();

	// dfs输出语法树
	// dfsTraverse(0, root);

	// 将符号存入符号表
	saveSymbol2table(root, &symTable);

	// printOutTable(&symTable);

	parseAllExp(root, &symTable);

	// dfsTraverse(0, root);

	printOutTable(&symTable);
	
	// 释放内存	
	// freeTableMemory(&symTable);
	// freeTreeMemory(root);

	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "Parse error at line %d, near token: %s\n", yylineno, yylval.npval->name);
	printf("Detail error message: ");
}