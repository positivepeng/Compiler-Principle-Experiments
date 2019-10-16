%option yylineno

two1 2

%%

{two1} {printf("two1");REJECT;}
"2" {printf("two2");}


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