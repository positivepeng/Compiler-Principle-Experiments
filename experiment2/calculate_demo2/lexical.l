%option noyywrap
%{
	#include "syntax.tab.h"
%}


digit [0-9]

%%

{digit}+ {return INT;}
"+" {return ADD;}
"-" {return SUB;}
"*" {return MUL;}
\\ {return DIV;}	

%%