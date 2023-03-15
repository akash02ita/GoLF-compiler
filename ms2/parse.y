%{
    #include <stdio.h>

    int yylex(void);
    void yyerror(char * err);

%}
%locations
%define parse.error custom

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
VarSpec : identifier Type


FunctionDecl : T_FUNC FunctionName Signature FunctionBody
FunctionName : identifier
FunctionBody : Block

Block: T_LC StatementList T_RC
StatementList : %empty
              | StatementList Statement T_S
Statement : Declaration
          | SimpleStmt
          | ReturnStmt 
          | BreakStmt 
          | Block  
          | IfStmt   
          | ForStmt
          
Signature : Parameters | Parameters Result
Result : Type
Type : TypeName
TypeName : identifier

Parameters  :  T_LP T_RP 
            | T_LP ParameterList T_RP 
            | T_LP ParameterList T_C T_RP
ParameterList : ParameterDecl
              | ParameterList T_C ParameterDecl
ParameterDecl : identifier Type

SimpleStmt : EmptyStmt
           | ExpressionStmt
           | Assignment

ExpressionStmt : Expression
Assignment : Expression assign_op Expression // this one seems ambigous: still compiles for now
assign_op : T_EQ

EmptyStmt : %empty
ReturnStmt: T_RET | T_RET Expression
BreakStmt: T_BREAK
IfStmt: T_IF Expression Block | T_IF Expression Block T_ELSE IfStmt | T_IF Expression Block T_ELSE Block
ForStmt: T_FOR Block | T_FOR Condition Block

Condition : Expression
Expression : pl2expr | Expression or_op pl2expr // or "||" has lowest precedence (precedence level 1 operator)
    pl2expr : pl3expr | pl2expr and_op pl3expr  // precedence level2 expression (the larger the precedene number the higher the priority to evaluate that expression first)
    pl3expr : pl4expr | pl3expr rel_op pl4expr
    pl4expr : pl5expr | pl4expr add_op pl5expr
    pl5expr : pl6expr | pl5expr mul_op pl6expr
    pl6expr : UnaryExpr

UnaryExpr  : PrimaryExpr | unary_op UnaryExpr
    or_op     : T_OR // precedence of level 1 (lowest priority in evalutation)
    and_op    : T_AND // precedence of level 2
    rel_op    : T_EE | T_NE | T_LT | T_LE | T_GT | T_GE // precedence of level 3
    add_op    : T_ADD | T_SUB // precedence of level 4
    mul_op    : T_MULT | T_DIV | T_MOD // precedence of level 5

    unary_op   : T_SUB | T_EMARK // precedence of level 6 (highest priority in evaluation)

PrimaryExpr : Operand | PrimaryExpr Arguments

Operand : Literal | OperandName | T_LP Expression T_RP
OperandName : identifier
Literal : BasicLit
BasicLit: int_lit | string_lit

Arguments : T_LP T_RP
          | T_LP ExpressionList T_RP
          | T_LP ExpressionList T_C T_RP
ExpressionList : Expression | ExpressionList T_C Expression

int_lit : T_INT
string_lit : T_STRING
identifier : T_ID
%%

// source method: jared's code
// seeing 'syntax error' only is not helpful. More info is needed
static int yyreport_syntax_error (const yypcontext_t *yyctx) {

    // Grab the string / current location of the unexpected token
    char const *unexpectedToken =
           // if there is no lookahead token (unexpected token), we return the
           // empty string.
        yypcontext_token(yyctx) == YYSYMBOL_YYEMPTY
            ? ""
            : yysymbol_name (yypcontext_token(yyctx));
    YYLTYPE unexpectedTokenLoc = *yypcontext_location(yyctx);

    // Again, you should use your own logging facilities!
    fprintf
        ( stderr
        , "Unexpected token `%s` at line %d. "
        , unexpectedToken
        , unexpectedTokenLoc.first_line
        );

    // Grab the tokens that bisoin expected (we only grab at most 10 of them)
    int const MAX_EXPECTED_TOKENS = 10;
    yysymbol_kind_t expectedSymbolKinds[MAX_EXPECTED_TOKENS];
    int actualNumberOfExpectedTokens = 
        yypcontext_expected_tokens
            ( yyctx
            , expectedSymbolKinds
            , MAX_EXPECTED_TOKENS
            );

    if (actualNumberOfExpectedTokens < 0) {
        // this may be negative on errors, so we forward such
        // errors to `yyparse`
        fprintf(stderr , "\n");
        return actualNumberOfExpectedTokens;
    }

    // read the documentation for why we should do this..
    if (actualNumberOfExpectedTokens == 0) {
        if (expectedSymbolKinds[0] != YYSYMBOL_YYEMPTY) {
            actualNumberOfExpectedTokens = MAX_EXPECTED_TOKENS;
        }
    }

    // two cases to get the english sentence to sort of make sense with the
    // punctuation i.e., want to have
    // > Expected `<token>`.
    // vs.
    // > Expected `<token>`, `<token>`, ..., or `<token>`.
    //
    // Note the punctuation...
    if (actualNumberOfExpectedTokens == 1) {
        fprintf(stderr, "Expected `%s`.\n", yysymbol_name(expectedSymbolKinds[0]));
    } else if (actualNumberOfExpectedTokens > 1) {
        fprintf(stderr, "Expected ");
        for (int i = 0; i + 1 < actualNumberOfExpectedTokens; ++i)
            fprintf(stderr, "`%s`, ", yysymbol_name(expectedSymbolKinds[i]));

        fprintf
            ( stderr
            , "or `%s`.\n"
            , yysymbol_name(expectedSymbolKinds[actualNumberOfExpectedTokens - 1])
            );
    }

    return 0; //error routine finished successfully, so return 0.
}

void yyerror(char* err) {
    fprintf(stderr, "%s\n", err);
}