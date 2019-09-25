%{
	#include<stdio.h>
%}

invalidletter [^0-9a-zA-Z]

%%

invalidletter {printf("invalid letter\n");}

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