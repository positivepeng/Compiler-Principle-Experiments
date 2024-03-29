/*! @file lex.l
 * @brief Lexical Analysis
 *********************************************************************
 * a simple calculator with variables
 * 
 * sample-files for a artikel in developerworks.ibm.com
 * Author: Christian Hagen, chagen@de.ibm.com
 * 
 * @par lex.l & lex.c
 * input for flex the lexical analysis generator
 * 
 *********************************************************************
 */
%option noyywrap

%{
/*--------------------------------------------------------------------
 * 
 * includes
 * 
 *------------------------------------------------------------------*/
    #include <stdio.h>
    #include "parse.tab.h"
/*--------------------------------------------------------------------
 * 
 * global variables
 * 
 *------------------------------------------------------------------*/

/*--------------------------------------------------------------------
 * 
 * defines
 * 
 *------------------------------------------------------------------*/
%}

YY "yy"
ZZ "zz"
SEMI ";" 

%%

{YY}+ {return YY;}
{ZZ}+ {return ZZ;}
{SEMI}+ {return SEMI;}

%%

/*--------------------------------------------------------------------
 * lex.l
 *------------------------------------------------------------------*/
