%option noyywrap
%option yylineno

%{
	#include <stdio.h>
	#include "tree.c"
	#include "project2.tab.h"
	#define YY_DECL int yylex()

	int output = 1;

	// 维护行号列号
	int yycolumn = 1;
	#define YY_USER_ACTION \
		yylloc.first_line = yylloc.last_line = yylineno; \
		yylloc.first_column = yycolumn; \
		yylloc.last_column = yycolumn + yyleng - 1; \
		yycolumn += yyleng;
%}


%%

"struct" {if(output) printf("STRUCT\n");return STRUCT;}
"return" {if(output) printf("RETURN\n");return RETURN;}
"if" {if(output) printf("IF\n");return IF;}
"else" {if(output) printf("ELSE\n");return ELSE;}
"while" {if(output) printf("WHILE\n");return WHILE;}
"int"|"float" {if(output) printf("TYPE\n");return TYPE;}

\/\/.*[\n] {printf("Comment:%s", yytext);}

[ \t]	{}; // ignore all whitespace
[\n]	{yycolumn = 1;}
[+-]?[0-9]+\.[0-9]+ 	{
	yylval.npval = newNode(FLOAT, yytext, yylloc.first_line, yylloc.first_column); 
	if(output) 
		printf("line:%d FLOAT:%f\n", yylloc.first_line, yylval.npval->val.fval); 
	return FLOAT;
}
[+-]?[0-9]+		{
	yylval.npval = newNode(INT, yytext, yylloc.first_line, yylloc.first_column); 
	if(output) 
		printf("line:%d INT:%d\n", yylloc.first_line, yylval.npval->val.ival); 
	return INT;
}
[a-zA-Z_][a-zA-Z_0-9]* {
	yylval.npval = newNode(ID, yytext, yylloc.first_line, yylloc.first_column); 
	if(output) 
		printf("line:%d ID %s\n", yylloc.first_line, yylval.npval->val.sval); 
	return ID;
}

; {if(output) printf("SEMI\n");return SEMI;}
, {if(output) printf("COMMA\n");return COMMA;}
= {if(output) printf("ASSIGNOP\n");return ASSIGNOP;}
>|<|>=|<=|==|!= {if(output) printf("RELOP\n");return RELOP;}
\+ {if(output) printf("PLUS\n");return PLUS;}
\- {if(output) printf("MINUS\n");return MINUS;}
\* {if(output) printf("STAR\n");return STAR;}
\\ {if(output) printf("DIV\n");return DIV;}
&& {if(output) printf("AND\n");return AND;}
\|\| {if(output) printf("OR\n");return OR;}
\. {if(output) printf("DOT\n");return DOT;}
\! {if(output) printf("NOT\n");return NOT;}
\( {if(output) printf("LP\n");return LP;}
\) {if(output) printf("RP\n");return RP;}
\[ {if(output) printf("LB\n");return LB;}
\] {if(output) printf("RB\n");return RB;}
\{ {if(output) printf("LC\n");return LC;}
\} {if(output) printf("RC\n");return RC;}
. {printf("undefined character %c\n",yytext[0]);}

%%