%{
	#include <stdio.h>
	extern int yylex();
	extern int yyparse();
	extern FILE* yyin;

	void yyerror(const char* s);
%}

%token INT
%token ADD SUB MUL DIV

%%	

Calc : /* empty */
	| Exp {printf("=%d\n", $2);}
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

int main() {
	yyin = stdin;

	do {
		yyparse();
	} while(!feof(yyin));

	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
}