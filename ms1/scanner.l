%{
#include<stdio.h>
enum {
  T_GT=1, T_GE, T_EQ, T_DOT, T_BEGIN, T_END, T_ID, T_BREAK, T_ELSE, T_FOR, T_FUNC, T_IF, T_RETURN, T_VAR, T_INT
};
int lineno = 0;
%}

%option noyywrap

newline               \n
ascii_char            .
ascii_letter          [a-zA-Z]
ascii_digit           [0-9]


letter                {ascii_letter}|_
decimal_digit         [0-9]

identifier            {letter}({letter}|{ascii_digit})

int_lit               {decimal_lit}
decimal_lit           {decimal_digits}
decimal_digits        {decimal_digit}{decimal_digit}*

%%

[ \t\r]+	            ; // ignore whitespace
\n			      {lineno++;} // keep track of line count (later more to do)

">"           return T_GT;
">="          return T_GE;
"="           return T_EQ;
"."           return T_DOT;

"if"          return T_IF;
"for"         return T_FOR;
"var"         return T_VAR;
"else"        return T_ELSE;
"func"        return T_FUNC;
"break"       return T_BREAK;
"return"      return T_RETURN;

{int_lit}   { fprintf(stdout, "integer %s detected\n", yytext); return T_INT; }

.           { fprintf(stderr, "unknown value %s\n", yytext); }
%% 
int main () {
  int token;
  while(token = yylex() != 0) {
    printf("Token value is %d from %s\n", token, yytext);
  }

}