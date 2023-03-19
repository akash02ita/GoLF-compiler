// %language "c++" // either this or the below line
%skeleton "lalr1.cc"
%require "3.8.1"

%define api.namespace {GoLF} 
%define api.parser.class {Parser} 
%defines

%locations 

%code requires{
    #include <memory>
    namespace GoLF {
        class Lexer;
    }
}

%parse-param {std::unique_ptr<GoLF::Lexer>& lexer}

%code {
    #include <iostream>
    #include <fstream>
    #include "lex.h"

    #undef yylex
    #define yylex lexer->yylex
}

%union {
    int num;
    const char * string; // const -> to avoid ISO C++ warning
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
%type <string> prog SourceFile TopLevelDecl Declaration VarDecl VarSpec FunctionDecl FunctionName FunctionBody Block Statement Signature Result Type TypeName Parameters ParameterList ParameterDecl SimpleStmt ExpressionStmt Assignment assign_op ReturnStmt BreakStmt IfStmt ForStmt Condition Expression pl2expr pl3expr pl4expr pl5expr pl6expr UnaryExpr or_op and_op rel_op add_op mul_op unary_op PrimaryExpr Operand OperandName Literal BasicLit Arguments ExpressionList int_lit string_lit identifier
%type <string> StatementList EmptyStmt
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

Block : T_LC StatementList T_RC
StatementList : %empty  {$$ = "";} // this is to avoid %type <string> StatementList 'empty warning'
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

EmptyStmt : %empty {$$ = ""; } // this is to avoid %type <string> EmptyStmt '%empty warning'
ReturnStmt : T_RET | T_RET Expression
BreakStmt : T_BREAK
IfStmt : T_IF Expression Block | T_IF Expression Block T_ELSE IfStmt | T_IF Expression Block T_ELSE Block
ForStmt : T_FOR Block | T_FOR Condition Block

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
BasicLit : int_lit | string_lit

Arguments : T_LP T_RP
          | T_LP ExpressionList T_RP
          | T_LP ExpressionList T_C T_RP
ExpressionList : Expression | ExpressionList T_C Expression

int_lit : T_INT
string_lit : T_STRING
identifier : T_ID
%%

void GoLF::Parser::error(const location_type& l, const std::string& m)
{
    std::cerr << "Error: " << m << " at " << l << "\n";
}
