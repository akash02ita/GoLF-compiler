%{
    #include <stdio.h>

    int yylex(void);
    void yyerror(char * err);

/*
this is the style on how to do EBNF IN bison
Suppose
    A -> {E}
    // IN BISON THE non-ambgious reduce/reduce way is as follows
    A -> %empty | AE

    A -> [E] // NOTHING TRICKY ABOUT IT just no(empty) or yes
    // in bison it would be
    A -> %empty | E

// other thing
    // while building grammar
        // each endpoint is best to terminate with different symbol
            // where the symbol is terminal
        // same %empty in all endpoints or same symbol may lead to 
            // ambiguity even if grammar is non-ambiguous 

EVERY ONCE IN A WIHLE KEEP CHECKING WITH `bison -Wcounterexamples parse.y`
if there is a shift reduce error that can be solved reight away instead of starting at completed code full of shift/reduce or reduce/reduce errors
*/
%}

%union {
    int num;
    char * string;
}

%token <string> T_BREAK
%token <string> T_ELSE
%token <string> T_FOR
%token <string> T_FUNC
%token <string> T_IF
%token <string> T_RET
%token <string> T_VAR
%token <string> T_ADD
%token <string> T_SUB
%token <string> T_MULT
%token <string> T_DIV
%token <string> T_MOD
%token <string> T_AND
%token <string> T_OR
%token <string> T_EE
%token <string> T_LT
%token <string> T_GT
%token <string> T_EQ
%token <string> T_EMARK
%token <string> T_NE
%token <string> T_LE
%token <string> T_GE
%token <string> T_LP
%token <string> T_RP
%token <string> T_LC
%token <string> T_RC
%token <string> T_C
%token <string> T_S
%token <string> T_ID
%token <string> T_INT
%token <string> T_STRING



%start prog
%type <string> SourceFile TopLevelDecl Declaration VarDecl FunctionDecl
%%
prog    : SourceFile
SourceFile	: %empty {fprintf(stdout, "empty sourcefile\n");}
			| SourceFile TopLevelDecl T_S { fprintf(stdout, "added %s %s\n", $2, $3);}

TopLevelDecl : Declaration | FunctionDecl
 Declaration : VarDecl
 VarDecl : T_VAR VarSpec 
 VarSpec : identifier type
  identifier : "a"
  type : "b"

FunctionDecl : T_FUNC FunctionName Signature FunctionBody
 FunctionName : identifier
 Signature : "c"
 FunctionBody : Block

%%

void yyerror(char* err) {
    fprintf(stderr, "%s\n", err);
}