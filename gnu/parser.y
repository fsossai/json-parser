%{

#include <stdio.h>

#define YYERROR_VERBOSE 1

int yylex();
void yyerror(char *s);

%}

%token T_STRING T_INT T_FLOAT T_BOOL T_NULL
%start file
%%

file
  : object
  | array
  ;

object
  : '{' '}'
  | '{' member_list '}'
  ;

member_list
  : member_list ',' member
  | member
  ;

member
  : name ':' value
  ;

array
  : '[' ']'
  | '[' value_list ']'
  ;

value_list
  : value_list ',' value
  | value
  ;

value
  : object
  | array
  | literal
  ;

literal
  : T_INT
  | T_FLOAT
  | T_STRING
  | T_BOOL
  | T_NULL
  ;

name
  : T_STRING
  ;

%%

void yyerror(char *s) { printf("%s\n", s); }

int main() {
  return yyparse();
}
