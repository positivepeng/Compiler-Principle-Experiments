%option noyywrap
%option yylineno

%{
	#include <stdio.h>
	#include "tree.c"
	#include "project2.tab.h"
	#define YY_DECL int yylex()

	int output = 0;

	// 维护行号列号
	int yycolumn = 1;
	#define YY_USER_ACTION \
		yylloc.first_line = yylloc.last_line = yylineno; \
		yylloc.first_column = yycolumn; \
		yylloc.last_column = yycolumn + yyleng - 1; \
		yycolumn += yyleng;
%}


%%

"struct" {
	yylval.npval = newNode(STRUCT, yytext, &yylloc); 
	if(output) printf("STRUCT\n");
	return STRUCT;
}
"return" {
	yylval.npval = newNode(RETURN, yytext, &yylloc); 
	if(output) printf("RETURN\n");
	return RETURN;
}
"if" {
	yylval.npval = newNode(IF, yytext, &yylloc); 
	if(output) printf("IF\n");
	return IF;
}
"else" {
	yylval.npval = newNode(ELSE, yytext, &yylloc); 
	if(output) printf("ELSE\n");
	return ELSE;
}
"while" {
	yylval.npval = newNode(WHILE, yytext, &yylloc); 
	if(output) printf("WHILE\n");
	return WHILE;
}
"int"|"float" {
	yylval.npval = newNode(TYPE, yytext, &yylloc); 
	if(output) printf("TYPE\n");
	return TYPE;
}

\/\/.*[\n] {printf("Comment:%s", yytext);}
\/\*(.|\n)*\*\/  {printf("Comment:%s", yytext);}

[ \t]	{}; // ignore all whitespace
[\n]	{yycolumn = 1;}
[+-]?[0-9]+\.[0-9]+ 	{
	yylval.npval = newNode(FLOAT, yytext, &yylloc); 
	if(output) 
		printf("line:%d FLOAT:%f\n", yylloc.first_line, yylval.npval->val.fval); 
	return FLOAT;
}
[+-]?[0-9]+		{
	yylval.npval = newNode(INT, yytext, &yylloc); 
	if(output) 
		printf("line:%d INT:%d\n", yylloc.first_line, yylval.npval->val.ival); 
	return INT;
}
[a-zA-Z_][a-zA-Z_0-9]* {
	yylval.npval = newNode(ID, yytext, &yylloc); 
	if(output) 
		printf("line:%d ID:%s\n", yylloc.first_line, yylval.npval->val.sval); 
	return ID;
}

; {
	yylval.npval = newNode(SEMI, yytext, &yylloc); 
	if(output) printf("SEMI\n");
	return SEMI;
}
, {
	yylval.npval = newNode(COMMA, yytext, &yylloc); 
	if(output) printf("COMMA\n");
	return COMMA;
}
= {
	yylval.npval = newNode(ASSIGNOP, yytext, &yylloc); 
	if(output) printf("ASSIGNOP\n");
	return ASSIGNOP;
}
>|<|>=|<=|==|!= {
	yylval.npval = newNode(RELOP, yytext, &yylloc); 
	if(output) printf("RELOP\n");
	return RELOP;
}
\+ {
	yylval.npval = newNode(PLUS, yytext, &yylloc); 
	if(output) printf("PLUS\n");
	return PLUS;
}
\- {
	yylval.npval = newNode(MINUS, yytext, &yylloc); 
	if(output) printf("MINUS\n");
	return MINUS;
}
\* {
	yylval.npval = newNode(STAR, yytext, &yylloc); 
	if(output) printf("STAR\n");
	return STAR;
}
\\ {
	yylval.npval = newNode(DIV, yytext, &yylloc); 
	if(output) printf("DIV\n");
	return DIV;
}
&& {
	yylval.npval = newNode(AND, yytext, &yylloc); 
	if(output) printf("AND\n");
	return AND;
}
\|\| {
	yylval.npval = newNode(OR, yytext, &yylloc); 
	if(output) printf("OR\n");
	return OR;
}
\. {
	yylval.npval = newNode(DOT, yytext, &yylloc); 
	if(output) printf("DOT\n");
	return DOT;
}
\! {
	yylval.npval = newNode(NOT, yytext, &yylloc); 
	if(output) printf("NOT\n");
	return NOT;
}
\( {
	yylval.npval = newNode(LP, yytext, &yylloc); 
	if(output) printf("LP\n");
	return LP;
}
\) {
	yylval.npval = newNode(RP, yytext, &yylloc); 
	if(output) printf("RP\n");
	return RP;
}
\[ {
	yylval.npval = newNode(LB, yytext, &yylloc); 
	if(output) printf("LB\n");
	return LB;
}
\] {
	yylval.npval = newNode(RB, yytext, &yylloc); 
	if(output) printf("RB\n");
	return RB;
}
\{ {
	yylval.npval = newNode(LC, yytext, &yylloc); 
	if(output) printf("LC\n");
	return LC;
}
\} {
	yylval.npval = newNode(RC, yytext, &yylloc); 
	if(output) printf("RC\n");
	return RC;
}

. {printf("undefined character %c\n",yytext[0]);}

%%