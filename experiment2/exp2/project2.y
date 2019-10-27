%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "tree.h"
	extern int yylex();
	extern int yyparse();
	extern FILE* yyin;

	void yyerror(const char* s);
%}

%union {
	int ival;
	float fval;
	struct NODE* npval;
}

%token<ival> INT
%token<fval> FLOAT
%token<npval> ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV AND OR DOT NOT TYPE
%token LP RP LB RB LC RC
%token STRUCT RETURN IF ELSE WHILE

%start Program

%%

Program : ExtDefList
;
ExtDefList : ExtDef ExtDefList
		|
;
ExtDef : Specifier ExtDecList SEMI
		| Specifier SEMI
		| Specifier FunDec CompSt
;
ExtDecList : VarDec
		| VarDec COMMA ExtDecList
;



Specifier : TYPE
		| StructSpecifier
;
StructSpecifier : STRUCT OptTag LC DefList RC
		| STRUCT Tag
;
OptTag : ID
	| 
;
Tag : ID
;



VarDec : ID
		| VarDec LB INT RB
;
FunDec : ID LP VarList RP
		| ID LP RP
;
VarList : ParamDec COMMA VarList
		| ParamDec
;
ParamDec : Specifier VarDec
;


CompSt : LC DefList StmtList RC
;
StmtList : Stmt StmtList
		| 
;
Stmt : Exp SEMI
		| CompSt
		| RETURN Exp SEMI
		| IF LP Exp RP Stmt
		| IF LP Exp RP Stmt ELSE Stmt
		| WHILE LP Exp RP Stmt
;



DefList : Def DefList
		|
;
Def : Specifier DecList SEMI
;
DecList : Dec
		| Dec COMMA DecList
;
Dec : VarDec
		| VarDec ASSIGNOP Exp
;


Exp : Exp ASSIGNOP Exp
		| Exp AND Exp
		| Exp OR Exp
		| Exp RELOP Exp
		| Exp PLUS Exp
		| Exp MINUS Exp
		| Exp STAR Exp
		| Exp DIV Exp
		| LP Exp RP
		| MINUS Exp
		| NOT Exp
		| ID LP Args RP
		| ID LP RP
		| Exp LB Exp RB
		| Exp DOT ID
		| Exp INT   // new
		| Exp FLOAT // new 
		| ID
		| INT
		| FLOAT
;

Args : Exp COMMA Args
		| Exp
;

%%

int main(int argc, char** argv) {
	
	if(argc > 1){
		if(!(yyin = fopen(argv[1], "r"))){
			perror(argv[1]);
			return 1;
		}
	}

	yyparse();
	
	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}