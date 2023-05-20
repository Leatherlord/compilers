%option noyywrap

%{
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "parser.tab.h"
#include "ast.h"

int line = 1;
void yyerror(char* str);

%}
%%
[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/] {}
"Var"       {return VAR; }
"Begin"     { return BBEGIN; }
"End"       { return EEND; }

"Repeat"    { return REPEAT; }
"Until"     { return UNTIL; }

";"         {  return SEMICOLON; }
","         {  return COMMA; }
"("         {  return LPARENTHENSIS; }
")"         {  return RPARENTHENSIS; }
"{"         {  return LBRACE; }
"}"         {  return RBRACE; }

"-"         { return MINUS; }
"+"         { return PLUS; }
"*"         { return MUL; }

"/"         { return DIV; }
">="        { return MORE; }
"=<"        { return LESS; }
":="         { return ASSIGN; }

"="        { return EQUALS; }

"not"       { return NOT; }

[a-zA-Z]+   {
                yylval.str = strdup(yytext);
                return ID;
            }

[0-9]+      {
                yylval.str = strdup(yytext);
                return CONST;
            }

"\n"        { line++; }

[ \t\r\f]+  { }

.           { yyerror("Unrecognized"); }
