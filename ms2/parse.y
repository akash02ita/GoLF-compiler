%{
    #include <stdio.h>

    int yylex(void);
    void yyerror(char * err);
%}

%token T_BREAK
%token T_ELSE
%token T_FOR
%token T_FUNC
%token T_IF
%token T_RET
%token T_VAR
%token T_ADD
%token T_SUB
%token T_MULT
%token T_DIV
%token T_MOD
%token T_AND
%token T_OR
%token T_EE
%token T_LT
%token T_GT
%token T_EQ
%token T_EMARK
%token T_NE
%token T_LE
%token T_GE
%token T_LP
%token T_RP
%token T_LC
%token T_RC
%token T_C
%token T_S
%token T_ID
%token T_INT
%token T_STRING

%%

start : fac | T_ID
    ;
fac : "a"
    ;
%%

void yyerror(char* err) {
    fprintf(stderr, "%s\n", err);
}