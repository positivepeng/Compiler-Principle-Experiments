%option yylineno

invalidChar [^\x20|\t|\n|\"|\'|#|,|=|>|<|>=|<=|==|!=|\+|\-|\*|\/|&&|\\|\||\.|\!|\(|\)|\[|\]|\{|\}|0-9|_|a-z|A-Z|;]
OctalNum 0[0-7]+
FloatNum [+-]?([1-9]+[0-9]*|[0-9]*\.[0-9]+|[1-9]+\.|\.[0-9]+)([eE]+[+-]?[0-9]+)?
HexadecimalNum 0[xX][0-9a-gA-G]+
ID [a-zA-Z_][0-9A-Za-z_]*
TenBaseNum [1-9]+[0-9]*
Num (0|0x|0X)?[.0-9a-zA-Z]+
%%

{ID}+ 	{}
0|{TenBaseNum}+ 		{}
{invalidChar}+ 		{printf("Error at line %d: Invalid character %s\n", yylineno, yytext);}
{OctalNum}+ 		{printf("Octal num : %s\n", yytext);}
{HexadecimalNum}+ 	{printf("Hexadecimal num : %s\n", yytext);}
{FloatNum}+ 		{printf("Float num : %s\n",yytext);}
{Num}+ 				{printf("Error at line %d: Invalid number %s\n", yylineno, yytext);}
.|\n 				{}


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