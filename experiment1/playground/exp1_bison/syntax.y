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

#include "lex.yy.c"
int main(int argc, char** argv){
	if(argc <= 1)
		return 1;
	FILE* f = fopen(argv[1], "r");
	if(!f){
		perror(argv[1]);
		return 1;
	}
	yyrestart(f);
	yyparse();
	return 0;
}

yyerror(char* msg){
	fprintf(stderr, "error: %s\n", msg);
}
