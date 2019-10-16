%{
	#include "syntax.tab.h"
%}


digit [0-9]

%%

{digit}+ {return INT;}
"+" {return PLUS;}
"-" {return SUB;}
"*" {return MUL;}
\\ {return DIV;}	

%%

int main(int argc, char** argv){
	if(argc > 1){
		if(!(yyin = fopen(argv[1], "r"))){
			perror(argv[1]);
			return 1;
		}
	}
	yylex();

	return 0;
}