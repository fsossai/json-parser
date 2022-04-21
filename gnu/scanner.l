%option noyywrap
%{
#include <stdio.h>
#include "parser.tab.h"
%}

%%

"true"|"false"                            { return T_BOOL;    }
"null"                                    { return T_NULL;    }
"0"|([1-9][0-9]*)                         { return T_INT;     }
"-"?[0-9]+(\.[0-9]+)?([eE]"-"?[0-9]+)?    { return T_FLOAT;   }
"\""([^\r\n\t]|"\\\"")*"\""               { return T_STRING;  }
[ \r\n\t]+                                {                   }
.                                         { return yytext[0]; }
