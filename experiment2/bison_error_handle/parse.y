%{
	#include <stdio.h>
	#include <stdlib.h>
	extern int yylex();
	extern int yyparse();
	extern FILE* yyin;
	extern int yylineno;
	void yyerror(const char* s);
%}


%token YY ZZ SEMI


%start slist

%% 

slist : slist stmt SEMI { printf("slist stmt\n"); }
      | stmt SEMI       { printf("stmt\n"); }
      | error SEMI      { printf("ERROR!!!\n");  yyerrok; }
      ;

stmt  : ZZ stmt
      | ZZ 
      ; 

%%
int main(int argc, char** argv) {
	
	if(argc > 1){
		if(!(yyin = fopen(argv[1], "r"))){
			perror(argv[1]);
			return 1;
		}
	}

	yydebug = 1;

	yyparse();
	
	return 0;
}

void yyerror(const char* msg) {
	/* 当语法分析遇到错误时自动调用 */
	printf("Error : %s\n", msg);
	// exit(1);
}