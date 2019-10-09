%{
	int chars = 0;
	int words = 0;
	int lines = 0;
	int invalidcount = 0;
%}

letter [a-zA-Z]


%%
{letter}+   {words++; chars += yyleng; }
\n {chars++; lines++;}
.  {chars++;}
%%


int main(int argc, char** argv){
	if(argc > 1){
		if(!(yyin = fopen(argv[1], "r"))){
			perror(argv[1]);
			return 1;
		}
	}
	yylex();
	printf("linecount : %8d\n", lines);
	printf("wordcount: %8d\n", words);
	printf("charactercount: %8d\n", chars);
	return 0;

}