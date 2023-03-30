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
%type <node> prog SourceFile TopLevelDecl Declaration VarDecl VarSpec FunctionDecl FunctionName FunctionBody Block Statement Signature Result Type TypeName Parameters ParameterList ParameterDecl SimpleStmt ExpressionStmt Assignment ReturnStmt BreakStmt IfStmt ForStmt Condition Expression pl2expr pl3expr pl4expr pl5expr pl6expr UnaryExpr PrimaryExpr Operand OperandName Literal BasicLit Arguments ExpressionList int_lit string_lit identifier
%type <node> StatementList EmptyStmt
%type <op> or_op and_op rel_op add_op mul_op unary_op
%type <op> assign_op
%%

prog    : SourceFile
SourceFile	: %empty {$$ = ""; } // do not print anything for now
			| SourceFile TopLevelDecl T_S { fprintf(stdout, "added %s %s\n", $2, $3);}

TopLevelDecl : Declaration | FunctionDecl
Declaration : VarDecl
VarDecl : T_VAR VarSpec 
VarSpec : identifier Type


FunctionDecl : T_FUNC FunctionName Signature FunctionBody
FunctionName : identifier
FunctionBody : Block

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

Statement : Declaration                         { $$ = newIdLine("Declaration", @$.first_line); } // for now create a dummy nodes only.
          | SimpleStmt                          { $$ = newIdLine("SimpleStmt", @$.first_line); } // for now create a dummy nodes only.
          | ReturnStmt                          { $$ = newIdLine("ReturnStmt", @$.first_line); } // for now create a dummy nodes only.
          | BreakStmt                           { $$ = newIdLine("BreakStmt", @$.first_line); } // for now create a dummy nodes only.
          | Block                           // { $$ = $1; } // this is default action
          | IfStmt                          { $$ = newBlockNoStmt(); } // for now create a dummy block only.
          | ForStmt                         // { $$ = $1; }
          
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
Assignment : Expression assign_op Expression    { $$ = newAssnStmt($1, $3, @$.first_line); }
assign_op : T_EQ    { $$ = EQ; }

EmptyStmt : %empty {$$ = ""; } // this is to avoid %type <string> EmptyStmt '%empty warning'
ReturnStmt : T_RET
           | T_RET Expression
BreakStmt : T_BREAK
IfStmt : T_IF Expression Block                                 // if statement
       | T_IF Expression Block T_ELSE IfStmt                   // if else statement (recursive)
       | T_IF Expression Block T_ELSE Block                    // if else statement
ForStmt : T_FOR Block                    { $$ = newForStmt(NULL, $2, @$.first_line); progTree = $$; }// condition = NULL, body = block
        | T_FOR Condition Block          { $$ = newForStmt($2, $3, @$.first_line); progTree = $$; } // condition = condition, body = block

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

Arguments : T_LP T_RP { $$ = newActual(NULL);}
          | T_LP ExpressionList T_RP { $$ = newActual(revLinkedList($2)); } // reverse $2 linked list
          | T_LP ExpressionList T_C T_RP { $$ = newActual(revLinkedList($2));} //  reverse $2 linked list
ExpressionList : Expression
               | ExpressionList T_C Expression { 
                    // To follow proper order append(ExpressionList) = Expression
                        // while temp.next != null. temp = temp.next
                        // temp.next = Expression
                    // But that is o(n^2) time complexity  (every time you append you need to recursive through whole thing)
                    // idea: Expression.next = ExpressionList (Expression is already evaluated) (So rather `prepend` and reverse later)
                    // then reverse whereever expressionlist is used (in Arguments)
                    if ($3->next != NULL) { // this should never happen
                        fprintf(stderr, "Error:ExpressionList: Expression.next is NULL\n");
                        exit(EXIT_FAILURE);
                    }
                    $3->next = $1;
                    $$ = $3;
                    } 

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