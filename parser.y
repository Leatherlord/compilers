%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
extern int yylex();
extern char* yyval;
extern int line;
void yyerror(char* str);

%}


%token VAR 
%token BBEGIN EEND
%token SEMICOLON COMMA LPARENTHENSIS RPARENTHENSIS LBRACE RBRACE
%token REPEAT UNTIL
%token MINUS
%token NOT
%token PLUS MUL DIV LESS MORE EQUALS
%token ASSIGN
%token ID CONST


%start program

%right ASSIGN
%left PLUS MINUS
%left MUL DIV

%union {
    struct Ast *tree;
    char *str;
}

%type<tree> program main_block variables_declaration variables operators operator expression sub_expression assigment complex_operator loop_operator compound_operator
%type<str> ID CONST
%%
program:
    variables_declaration main_block { $$ = add_node(Root, NULL, $1, $2); }
    ;

main_block:
    BBEGIN operators EEND { $$ = add_node(Main_block, NULL, $2, NULL); }
    ;

variables_declaration:
    VAR variables SEMICOLON { $$ = add_node(Variables_Declaration, NULL, NULL, $2); }
    ;

variables:
    ID { $$ = add_variable($1, NULL); }
    | ID COMMA variables { $$ = add_variable($1, $3); }
    ;

operators:
    operator { $$ = $1; }
    | operator operators { $$ = add_node(Operators, NULL, $1, $2); }
    ;

operator:
    assigment { $$ = $1;}
    | complex_operator { $$ = $1;}
    ;

assigment:
    ID ASSIGN expression SEMICOLON { $$ = add_assignment($1, $3); }
    ;

expression:
    MINUS sub_expression { $$ = add_node(Expression, "MINUS", NULL, $2); }
    | NOT sub_expression { $$ = add_node(Expression, "NOT", NULL, $2); }
    | sub_expression { $$ = $1; }
    ;

sub_expression:
    LPARENTHENSIS expression RPARENTHENSIS { $$ = $2; }
    | ID { $$ = get_variable($1); }
    | CONST { $$ = get_constant($1); }
    | sub_expression PLUS sub_expression { $$ = add_node(Expression, "+", $1, $3); }
    | sub_expression MINUS sub_expression { $$ = add_node(Expression, "-", $1, $3); }
    | sub_expression MUL sub_expression { $$ = add_node(Expression, "*", $1, $3); }
    | sub_expression DIV sub_expression { $$ = add_node(Expression, "/", $1, $3); }
    | sub_expression EQUALS sub_expression { $$ = add_node(Expression, "=", $1, $3); }
    | sub_expression MORE sub_expression { $$ = add_node(Expression, ">=", $1, $3); }
    | sub_expression LESS sub_expression { $$ = add_node(Expression, "=<", $1, $3); }
    ;

complex_operator:
    loop_operator { $$ = $1;}
    | compound_operator { $$ = $1;}
    ;

loop_operator:
    REPEAT operators UNTIL expression { $$ = add_node(Loop, NULL, $2, $4); }
    ;
compound_operator:
    LBRACE operators RBRACE { $$ = add_node(Operators, NULL, $2, NULL); }
    ;

%%

void yyerror(char* str) {
    fprintf(stderr, "error at %d line\n", line);
    exit(1);
}

int main(int argc, char **argv) {
    return yyparse();
}

