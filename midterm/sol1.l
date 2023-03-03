%option noyywrap
%{
    #include <stdio.h>
    #define true 1
    #define false 0
    #define STOP -1
%}

int_lit {decimal_lit}|{binary_lit}
decimal_lit 0|[1-9](_?{decimal_digits})?
binary_lit 0(b|B)_?{binary_digits}

decimal_digits [0-9](_?[0-9])*
binary_digits [0-1](_?[0-1])*

%%

^{int_lit}$           {fprintf(stdout, "Integer literal matched: %s\n", yytext); return true;}

.*                   {fprintf(stdout, "Invalid input %s\n",yytext); return false;}

<INITIAL><<EOF>>     {fprintf(stdout,"EOF reached\n"); return STOP;}
%%

int main() {
    // use stdin by default: every new string input will work
    // press CNTRL+D for halting program
    while (yylex() != STOP);
    return 0;
}