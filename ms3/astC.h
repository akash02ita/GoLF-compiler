#ifndef AST_H
#define AST_H
/*******************************
 * Shankar Ganesh
 * C AST for GoLF
 * Based on my AST for J- 
 * written for CPSC 411 Winter 
 * 2020
 ********************************/

/* https://github.com/llvm/llvm-project/issues/55274
   all the code could also be defined inline in header file without compiling.
   However, when using `gcc` prefix `extern inline` is needed (not just inline).
   using `static` might also work.
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_CHILDREN 3 

typedef enum
{
    ADD, SUB, MULT, DIV, MOD,
    LT, GT, LTE, GTE, EQ, NEQ,
    AND, OR, EMARK, EQEQ
} Oper;

typedef enum 
{
    INT, BOOL, STR, VOID
} TypeName;

typedef enum {Prog, Stmt, Expr, Decl} NodeType;
typedef enum {IfStmt, IfElseStmt, Block, Empty, Assn, For, ExprStmt, DeclStmt, 
    BreakStmt, ReturnStmt} StmtType;

// TODO: should i add newId type as well?
/* whenever doing newvardecl or newglobdecl (so go in parse.y)
    Declaration: T_VAR identifier Type 
        replace identifier with T_ID and use a new method newNewIdLine() to make an id of enum `newId`
*/
typedef enum {Id, TypeId, BasicLit, UnaryExp, BinaryExp, FuncCall, Actual, Actuals} ExprType;
typedef enum {FuncDecl, VarDecl, GlobVarDecl, ParamDecl, Params, Signature} DeclType;

typedef struct ASTNode 
{
    const char* label;
    struct ASTNode* children[MAX_CHILDREN];
    struct ASTNode* next; // or sibling 
    NodeType node_type;

    union
    {
        StmtType stmt;
        ExprType exp;
        DeclType decl;
    } kind;
    
    union
    {
        Oper op;
        int ival;
        char* sval;
    } val;

    TypeName type_name;
    int line;

}ASTNode;

extern ASTNode * progTree;

const char* opToString(Oper op);

const char* typeToStr(TypeName type);

//--------------------------------
// Program 
// -------------------------------
ASTNode* newProg(const char* filename);

//--------------------------------
// Statement 
// -------------------------------
ASTNode* newIfStmt(ASTNode* clause, ASTNode* body, int line);

ASTNode* newIfElseStmt(ASTNode* clause, ASTNode* ifbody, ASTNode* elsebody, int line);

ASTNode* newBlockStmt(ASTNode* stmt);

ASTNode* newBlockNoStmt();

ASTNode* newEmptyStmt();

ASTNode* newAssnStmt(ASTNode* lhs, ASTNode* rhs, int line);

ASTNode* newForStmt(ASTNode* cond, ASTNode* body, int line);

ASTNode* newExprStmt(ASTNode* expr);

ASTNode* newDeclStmt(ASTNode* decl);

ASTNode* newBrkStmt(int line);

ASTNode* newRetStmt(int line);

ASTNode* newRetExprStmt(ASTNode* expr, int line);

//--------------------------------
// Expressions 
// -------------------------------
ASTNode* newId(char *name);

ASTNode* newIdLine(char *name, int line);

ASTNode* newTypeId(char* name);

ASTNode* newTypeIdLine(char* name, int line);

ASTNode* newIntBasicLit(int value, int line);

ASTNode* newStrBasicLit(char* value, int line);

ASTNode* newUnExp(Oper op, ASTNode* right, int line);

ASTNode* newBinExp(Oper op, ASTNode* left, ASTNode* right, int line);

ASTNode* newFuncCall(ASTNode* name, ASTNode* args);

ASTNode* newActual(ASTNode* expr);

ASTNode* newActuals();

//--------------------------------
// Declaratations
// -------------------------------
ASTNode* newFuncDecl(ASTNode* id, ASTNode* sig, ASTNode* body, int line);


ASTNode* newGlobVarDecl(ASTNode* id, ASTNode* type, int line);
ASTNode* newVarDecl(ASTNode* id, ASTNode* type, int line);

ASTNode* newParamDecl(ASTNode *id, ASTNode* type);

ASTNode* newParams();

ASTNode* newSignature(ASTNode* type, ASTNode* params);


// NOTE: Parameters and actuals are done this way to 
// save memory. Most nodes will not have so many children
// and this prevents having a max number of arguments/params/statements in a block

void append(ASTNode* stmt1, ASTNode* stmt2);

void addActual(ASTNode* parent, ASTNode* actual);

void addParam(ASTNode* parent, ASTNode* param);

// Place printTree somewhere else in a C file and remove 
void printTree(ASTNode* tree, FILE* out,int INDENT);


/*
    TODO: do i need to add more fields to annotate ast?
    TODO: should i put another method to printAnnotedTree? Main.c will use this method instead after it
*/

void preTraversal(ASTNode* tree, void (* preFunc)(ASTNode * tree), int (* haltFunc)(ASTNode * tree));
void prePostTraversal(ASTNode* tree, void (* preFunc)(ASTNode * tree), void (* postFunc)(ASTNode * tree), int (* haltFunc)(ASTNode * tree));
void postTraversal(ASTNode* tree, void (* postFunc)(ASTNode * tree), int (* haltFunc)(ASTNode * tree));

#endif
