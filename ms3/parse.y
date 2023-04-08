%code requires{
    // instead of %{ %}, %code requires {} fixes all %union issues. Try to replace %code requires with before, and you will see the problem when compiling with `make`.
    #include <stdio.h>
    #include "astC.h"
    int yylex(void);
    void yyerror(char * err);
    ASTNode * revLinkedList(ASTNode * node);
}
%locations
%define parse.error custom

%union {
    int num;
    char * string;
    ASTNode * node; // %code requires fixes the problme
    Oper op; // %code requires fixes the problme
}

%token <string> T_BREAK T_ELSE T_FOR T_FUNC T_IF T_RET T_VAR T_ADD T_SUB T_MULT T_DIV T_MOD T_AND T_OR T_EE T_LT T_GT T_EQ T_EMARK T_NE T_LE T_GE T_LP T_RP T_LC T_RC T_C T_S T_ID T_INT T_STRING



%start prog
// %start Expression
%type <node> prog SourceFile TopLevelDecl Declaration GlobarVarDeclaration FunctionDecl FunctionName FunctionBody Block Statement Signature Result Type TypeName Parameters ParameterList ParameterDecl SimpleStmt ExpressionStmt Assignment ReturnStmt BreakStmt IfStmt ForStmt Condition Expression pl2expr pl3expr pl4expr pl5expr pl6expr UnaryExpr PrimaryExpr Operand OperandName Literal BasicLit Arguments ExpressionList int_lit string_lit identifier
%type <node> StatementList EmptyStmt
%type <op> or_op and_op rel_op add_op mul_op unary_op
%type <op> assign_op
%%

prog    : SourceFile                            { $$ = $1; progTree = $$; }
SourceFile	: %empty                            { $$ = NULL; }
			| SourceFile TopLevelDecl T_S       { if ($$ == NULL) $$ = $2; else {$$ = $1; append($$, $2); } }

TopLevelDecl : GlobarVarDeclaration             // { $$ = $1; }   // Declaration
             | FunctionDecl                     // { $$ = $1; }

GlobarVarDeclaration: T_VAR identifier Type     { $$ = newGlobVarDecl($2, $3, @$.first_line); }
Declaration: T_VAR identifier Type              { $$ = newVarDecl($2, $3, @$.first_line); }
// Declaration : VarDecl
// VarDecl : T_VAR VarSpec 
// VarSpec : identifier Type


FunctionDecl : T_FUNC FunctionName Signature FunctionBody  { $$ = newFuncDecl($2, $3, $4, @$.first_line); }
FunctionName : identifier   // { $$ = $1; }
FunctionBody : Block        // { $$ = $1; }

Signature : Parameters          { ASTNode * type = newTypeIdLine("void", @$.first_line); $$ = newSignature(type, $1); }
          | Parameters Result   { $$ = newSignature($2, $1); }
Result : Type                // { $$ = $1; }

Parameters  :  T_LP T_RP                            { $$ = newParams(); }
            | T_LP ParameterList T_RP               { $$ = $2; }
            | T_LP ParameterList T_C T_RP           { $$ = $2; }
ParameterList : ParameterDecl                       { $$ = newParams(); addParam($$, $1); }
              | ParameterList T_C ParameterDecl     { $$ = $1; addParam($$, $3); }
ParameterDecl : identifier Type                     { $$ = newParamDecl($1, $2); }

// version1 of Block: works
// %empty calls multiple times. if you remove revLinkedList and replac $$ = NULL to like $$ = newId("hello"), hello will be shown multiple times. (tested with for loop example)
// Block : T_LC StatementList T_RC                 { $$ = newBlockStmt(revLinkedList($2)); }// statementlist only used and belongs to block: reverse linked list here for efficiency
// StatementList : %empty                          { $$ = NULL; }  // { $$ = newId("hello"); }         //  newblock with no statement (newBlockNoStmt();). If grammar was Block: ( nonemptyStatementList ) and Block : () then newblocknostmt would be used. Here we don't need to. see other version
//               | StatementList Statement T_S     { if ($2 == NULL) fprintf(stderr, "Error:StatementList: Statement is null\n");
//                                                   $2->next = $1; $$ = $2;
//                                                 } // block with statement: NOTE prepend for efficiency $2 = newblokc($1)

// version 2 of Block: works but looks better (%empty on version1 triggers multiple times. although $$=NULL should not cause issues.)
Block : T_LC T_RC                            { $$ = newBlockNoStmt(); }
      | T_LC StatementList T_RC              { $$ = newBlockStmt(revLinkedList($2)); }
StatementList : Statement T_S                { $$ = $1; }
              | StatementList Statement T_S  { $2->next = $1; $$ = $2;}

Statement : Declaration                     // { $$ = $1; }
          | SimpleStmt                      // { $$ = $1; }
          | ReturnStmt                      // { $$ = $1; }
          | BreakStmt                       // { $$ = $1; }
          | Block                           // { $$ = $1; } // this is default action
          | IfStmt                          // { $$ = $1; }
          | ForStmt                         // { $$ = $1; }
          
Type : T_ID                            { $$ = newTypeIdLine($1, @$.first_line); }
// Type : TypeName                            { $$ = newTypeIdLine($1->val.sval, $1->line); } // works but incovenient
// TypeName : identifier                      // { $$ = $1; }
// Type : TypeName                            { $$ = newTypeIdLine($1, @$.first_line); } // does not work as $1 is a ASTNODE * and not char *
// TypeName : identifier                      // { $$ = $1; }


SimpleStmt : EmptyStmt                          // { $$ = $1; }
           | ExpressionStmt                     // { $$ = $1; }
           | Assignment                         // { $$ = $1; }

ExpressionStmt : Expression                     { $$ = newExprStmt($1); }
Assignment : Expression assign_op Expression    { $$ = newAssnStmt($1, $3, @$.first_line); }
assign_op : T_EQ                                { $$ = EQ; }

EmptyStmt : %empty                              { $$ = newEmptyStmt(); }
ReturnStmt : T_RET                              { $$ = newRetStmt(@$.first_line); }
           | T_RET Expression                   { $$ = newRetExprStmt($2, @$.first_line); }
BreakStmt : T_BREAK                             { $$ = newBrkStmt(@$.first_line); }
IfStmt : T_IF Expression Block                         { $$  = newIfStmt($2, $3, @$.first_line); }              // if statement
       | T_IF Expression Block T_ELSE IfStmt           { $$  = newIfElseStmt($2, $3, $5, @$.first_line); }      // if else statement (recursive)
       | T_IF Expression Block T_ELSE Block            { $$  = newIfElseStmt($2, $3, $5, @$.first_line); }      // if else statement
ForStmt : T_FOR Block                    { $$ = newForStmt(NULL, $2, @$.first_line); }// condition = NULL, body = block
        | T_FOR Condition Block          { $$ = newForStmt($2, $3, @$.first_line); } // condition = condition, body = block

Condition : Expression
Expression : pl2expr                    // default action:{ $$ = $1; }
           | Expression or_op pl2expr   { $$ = newBinExp($2, $1, $3, @$.first_line); } // or "||" has lowest precedence (precedence level 1 operator)
    pl2expr : pl3expr 
            | pl2expr and_op pl3expr    { $$ = newBinExp($2, $1, $3, @$.first_line); } // precedence level2 expression (the larger the precedene number the higher the priority to evaluate that expression first)
    pl3expr : pl4expr 
            | pl3expr rel_op pl4expr     { $$ = newBinExp($2, $1, $3, @$.first_line); }
    pl4expr : pl5expr 
            | pl4expr add_op pl5expr     { $$ = newBinExp($2, $1, $3, @$.first_line); }
    pl5expr : pl6expr 
            | pl5expr mul_op pl6expr     { $$ = newBinExp($2, $1, $3, @$.first_line); }
    pl6expr : UnaryExpr

UnaryExpr  : PrimaryExpr
           | unary_op UnaryExpr    { $$ = newUnExp($1, $2,  @$.first_line); }
    or_op     : T_OR { $$ = OR; }// precedence of level 1 (lowest priority in evalutation)
    and_op    : T_AND { $$ = AND; } // precedence of level 2
    rel_op    : T_EE  { $$ = EQEQ; } 
              | T_NE  { $$ = NEQ; } 
              | T_LT  { $$ = LT; } 
              | T_LE  { $$ = LTE; } 
              | T_GT  { $$ = GT; } 
              | T_GE  { $$ = GTE; } // precedence of level 3
    add_op    : T_ADD { $$ = ADD; }
              | T_SUB { $$ = SUB; } // precedence of level 4
    mul_op    : T_MULT { $$ = MULT; } 
              | T_DIV  { $$ = DIV; }
              | T_MOD  { $$ = MOD; } // precedence of level 5

    unary_op   :T_SUB       { $$ = SUB; }
               | T_EMARK    { $$ = EMARK; }

PrimaryExpr : Operand
            | PrimaryExpr Arguments  { $$ = newFuncCall($1, $2); } // ????????

Operand : Literal
        | OperandName
        | T_LP Expression T_RP   { $$ = $2; } // { $$ = newActual($2);  } // ??????????
OperandName : identifier
Literal : BasicLit
BasicLit : int_lit  // { fprintf(stdout, "intlit %d\n", $1->val.ival); }
         | string_lit  // { fprintf(stdout, "strlit %s\n", $1->val.sval); }

Arguments : T_LP T_RP { $$ = newActuals();}
          | T_LP ExpressionList T_RP { $$ = $2; }
          | T_LP ExpressionList T_C T_RP { $$ = $2;}
ExpressionList : Expression                    { $$ = newActuals(); addActual($$, $1); }
               | ExpressionList T_C Expression { $$ = $1; addActual($$, $3); }

int_lit : T_INT                 { $$ = newIntBasicLit(atoi($1), @$.first_line); }
string_lit : T_STRING           { $$ = newStrBasicLit($1, @$.first_line); }
identifier : T_ID               { $$ = newIdLine($1, @$.first_line); }
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

ASTNode * revLinkedList(ASTNode * node) {
    ASTNode * prev = NULL;  
    ASTNode * next = NULL;  
    ASTNode * curr = node;
    while (curr != NULL) {
        next = curr->next;
        curr->next = prev;
        
        prev = curr;
        curr = next;
    }
    
    return prev; // prev is the new head
}