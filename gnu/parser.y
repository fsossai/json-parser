%{
#include <stdio.h>

extern int yylineno;

int  yylex();
void yyerror(const char *s);
%}

%define parse.error detailed
%define api.pure full

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

void yyerror(const char *str) {
  fprintf(stderr,"%s in line %d\n", str, yylineno);
}

int main() {
  return yyparse();
}
