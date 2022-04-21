%option noyywrap yylineno
%{
#include <stdio.h>
#include "parser.tab.h"
%}

escape      "\\"[bfnrt\"\\/]
unicode     "\\u"[A-Fa-f0-9]{4}
special     {escape}|{unicode}
exp         [eE][+-]?[0-9]+
integer     "-"?("0"|[1-9][0-9]*)

%%

"true"|"false"                      { return T_BOOL;    }
"null"                              { return T_NULL;    }
{integer}                           { return T_INT;     }
{integer}(\.[0-9]+)?{exp}?          { return T_FLOAT;   }
"\""([^\r\n\t\\\"]|{special})*"\""  { return T_STRING;  }
[ \r\n\t]+                          {                   }
.                                   { return yytext[0]; }
