%option yylineno
%{
	int num_invalid = 0;
	char errorChar;
	int errorCharLinenNum;
%}


SEMI ;
COMMA ,
ASSIGNOP =
RELOP  >|<|>=|<=|==|!=
PLUS +
MINUS -
STAR *
DIV /
AND &&
OR ||
DOT .
NOT !
LP (
RP )
LB [
RB ]
LC {
RC }
invalidChar [^\x20|\t|\n|\"|\'|#|,|=|>|<|>=|<=|==|!=|\+|\-|\*|\/|&&|\\|\||\.|\!|\(|\)|\[|\]|\{|\}|0-9|_|a-z|A-Z|;]
OctalNum 0[0-7]+
FloatNum [+-]?([0-9]*\.?[0-9]+|[0-9]+\.)[eE]?[+-]?[0-9]+
HexadecimalNum 0x[0-9a-gA-G]+
VariableName [a-zA-Z_]+[0-9A-Za-z]*
DigitNum [1-9]+[0-9]*
Num (0|0x)?[.0-9a-gA-G]+
%%

{VariableName}+ {printf("VariableName : %s\n", yytext);}
{DigitNum}+ {printf("DigitNum : %s\n", yytext);}
{invalidChar}+ {num_invalid++; errorChar = yytext[0]; errorCharLinenNum = yylineno;}
{OctalNum}+ {printf("Octal num : %s\n", yytext);}
{FloatNum}+ {printf("Float num : %s\n", yytext);}
{HexadecimalNum}+ {printf("Hexadecimal num : %s\n", yytext);}
{Num}+ {printf("Invalid num : %s\n", yytext);}
.|\n {;}


%%

int main(int argc, char** argv){
	if(argc > 1){
		if(!(yyin = fopen(argv[1], "r"))){
			perror(argv[1]);
			return 1;
		}
	}
	yylex();

	// 能识别词法中未定义的字符
	if(num_invalid > 0){
		printf("Error at line %d: Invalid character %c\n", errorCharLinenNum, errorChar);
	}

	// 能识别浮点数


	return 0;
}