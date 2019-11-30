%option noyywrap
%option yylineno


%{
	int output = 0;
%}


INT [+-]?[1-9][0-9]*
FLOAT [+-]?([0-9]*\.?[0-9]+|[0-9]+\.)
ID [a-zA-Z_][0-9a-zA-Z_]*

OctalNum 0[0-7]+       			
IllegalOctalNum 0[0-9]*
HexadecimalNum 0[xX][0-9a-fA-F]+
IllegalHexadecimalNum 0[xX][0-9a-zA-Z]*
FloatingPointNum [+-]?([1-9]+[0-9]*|[0-9]*\.[0-9]+|[1-9]+\.|\.[0-9]+)([eE]+[+-]?[0-9]+)?
IllegalFloatingPointNum [+-]?[.eE0-9]*
COMMENT1 \/\/.*\n
COMMENT2 \/\*(.|\n)*\*\/


SEMI ;
COMMA ,
ASSIGNOP =
RELOP >|<|>=|<=|==|!=
PLUS \+
MINUS \-
STAR \*
DIV \\
AND &&
OR \|\|
DOT \.
NOT \!
TYPE "int"|"float"
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}
STRUCT "struct"
RETURN "return"
IF "if"
ELSE "else"
WHILE "while"

%% 

{TYPE}+ {if(output) printf("TYPE\n");}
{STRUCT}+ {if(output) printf("STRUCT\n");}
{RETURN}+ {if(output) printf("RETURN\n");}
{IF}+ {if(output) printf("IF\n");}
{ELSE}+ {if(output) printf("ELSE\n");}
{WHILE}+ {if(output) printf("WHILE\n");}

0|{INT}+ {if(output) printf("INT\n");}
{OctalNum}+ {printf("Octal number : %s\n", yytext);}
{IllegalOctalNum}+ {printf("Error at Line %d: Illegal octal number \'%s\'.\n", yylineno, yytext);}
{HexadecimalNum}+ {printf("Hexadecimal number : %s\n", yytext);}
{IllegalHexadecimalNum}+ {printf("Error at Line %d: Illegal hexadecimal number \'%s\'.\n", yylineno, yytext);}
{FloatingPointNum} {printf("Floating point number : %s\n", yytext);}
{IllegalFloatingPointNum} {printf("Error at Line %d: Illegal floating point number \'%s\'.\n", yylineno, yytext);}
{COMMENT1}+ {printf("Comment at Line %d: %s", yylineno ,yytext);}
{COMMENT2}+ {printf("Comment at Line %d: %s\n", yylineno ,yytext);}


{FLOAT}+ {if(output) printf("FLOAT\n");}
{ID}+ {if(output) printf("ID\n");}
{SEMI}+ {if(output) printf("SEMI\n");}
{COMMA}+ {if(output) printf("COMMA\n");}
{ASSIGNOP}+ {if(output) printf("ASSIGNOP\n");}
{RELOP}+ {if(output) printf("RELOP\n");}
{PLUS}+ {if(output) printf("PLUS\n");}
{STAR}+ {if(output) printf("STAR\n");}
{DIV}+ {if(output) printf("DIV\n");}
{AND}+ {if(output) printf("AND\n");}
{OR}+ {if(output) printf("OR\n");}
{LP}+ {if(output) printf("LP\n");}
{RP}+ {if(output) printf("RP\n");}
{LB}+ {if(output) printf("LB\n");}
{RB}+ {if(output) printf("RB\n");}
{LC}+ {if(output) printf("LC\n");}
{RC}+ {if(output) printf("RC\n");}
\x20|\t|\n {}
. { printf("Error at Line %d: Mysterious characters \'%s\'\n",yylineno, yytext);}
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