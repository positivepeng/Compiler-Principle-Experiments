%{
	#include <stdio.h>	
	#include "lex.yy.c"
%}

%token INT
%token ADD SUB MUL DIV

%%

Calc : /* empty */
	| Exp {printf("= %d\n", $1);}
	;

Exp : Factor
	| Exp ADD Factor { $$ = $1 + $3; }
	| Exp SUB Factor { $$ = $1 - $3; }
	;

Factor : Term
	| Factor MUL Term { $$ = $1 * $3; }
	| Factor DIV Term { $$ = $1 / $3; }
	;

Term : INT
	;

%%