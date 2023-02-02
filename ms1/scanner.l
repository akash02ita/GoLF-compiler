%{
#include<stdio.h>
int lineno = 0;
%}

%option noyywrap

int_lit               {decimal_lit}
decimal_lit           {decimal_digits}
decimal_digits        {decimal_digit}{decimal_digit}*
decimal_digit         [0-9]



%%

\n          { lineno++; }
{int_lit}   { fprintf(stdout, "integer %s detected\n", yytext); return 0; }

%% 
int main () {

  while(1) {yylex();}

}