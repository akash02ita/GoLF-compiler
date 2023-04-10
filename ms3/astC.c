/*******************************
 * Shankar Ganesh
 * C AST for GoLF
 * Based on my AST for J- 
 * written for CPSC 411 Winter 
 * 2020
 ********************************/



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "astC.h"

ASTNode * progTree;

const char* opToString(Oper op)
{
    switch (op) 
    {
    case ADD:       return "+";
    case SUB:       return "-";
    case MULT:      return "*";
    case DIV:       return "/";
    case MOD:       return "%";
    case LT:        return "<";
    case GT:        return ">";
    case LTE:       return "<=";
    case GTE:       return ">=";
    case EQ:        return "==";
    case NEQ:       return "!=";
    case EMARK:     return "!";
    case EQEQ:      return "==";
    default:        return "";
    }
}

const char* typeToStr(TypeName type)
{
    switch (type) 
    {
    case INT:       return "INT";
    case BOOL:      return "BOOL";
    case STR:       return "STR";
    case VOID:      return "VOID";
    default:        return "";
    }
}

//--------------------------------
// Program 
// -------------------------------
ASTNode* newProg(const char* filename)
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create program node");
        return node;
    }

    node->label = strdup(filename);
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Prog;
    return node;
}

//--------------------------------
// Statement 
// -------------------------------
ASTNode* newIfStmt(ASTNode* clause, ASTNode* body, int line)
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create IfStmt node");
        return node;
    }

    node->label = "IfStmt;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Stmt;
    node->kind.stmt = IfStmt;
    node->children[0] = clause;
    node->children[1] = body;
    node->line = line;
    return node;
}

ASTNode* newIfElseStmt(ASTNode* clause, ASTNode* ifbody, ASTNode* elsebody, int line)
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create IfElseStmt node");
        return node;
    }

    node->label = "IfElseStmt;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Stmt;
    node->kind.stmt = IfStmt;
    node->children[0] = clause;
    node->children[1] = ifbody;
    node->children[2] = elsebody;
    node->line = line;
    return node;
}

ASTNode* newBlockStmt(ASTNode* stmt) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create BlockStmt node");
        return node;
    }

    node->label = "Block;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Stmt;
    node->kind.stmt = Block;
    node->children[0] = stmt; // Access siblings using next of this child
    return node;
}

 ASTNode* newBlockNoStmt() 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create BlockStmt node");
        return node;
    }

    node->label = "Block;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Stmt;
    node->kind.stmt = Block;
    return node;
}

 ASTNode* newEmptyStmt() 
{

    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create EmptyStmt node");
        return node;
    }

    node->label = "EmptyStmt;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Stmt;
    node->kind.stmt = Block;
    return node;
}

 ASTNode* newAssnStmt(ASTNode* lhs, ASTNode* rhs, int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create AssnStmt node");
        return node;
    }

    node->label = "AssnStmt;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Stmt;
    node->kind.stmt = Assn;
    node->line = line;
    node->children[0] = lhs;
    node->children[1] = rhs;
    node->val.op = EQ;
    return node;
}

 ASTNode* newForStmt(ASTNode* cond, ASTNode* body, int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create ForStmt node");
        return node;
    }

    node->label = "ForStmt;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Stmt;
    node->kind.stmt = For;
    node->line = line;
    node->children[0] = cond;
    node->children[1] = body;
    return node;
}

 ASTNode* newExprStmt(ASTNode* expr) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create ExprStmt node");
        return node;
    }

    node->label = "ExprStmt;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Stmt;
    node->kind.stmt = ExprStmt;
    node->children[0] = expr;
    return node;
}

 ASTNode* newDeclStmt(ASTNode* decl) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create DeclStmt node");
        return node;
    }

    node->label = "DeclStmt;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Stmt;
    node->kind.stmt = DeclStmt;
    node->children[0] = decl;
    return node;
}

 ASTNode* newBrkStmt(int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create BrkStmt node");
        return node;
    }

    node->label = "BreakStmt;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Stmt;
    node->kind.stmt = BreakStmt;
    node->line = line;
    return node;
}

 ASTNode* newRetStmt(int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create ReturnStmt node");
        return node;
    }

    node->label = "ReturnStmt;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Stmt;
    node->kind.stmt = ReturnStmt;
    node->line = line;
    return node;
}

 ASTNode* newRetExprStmt(ASTNode* expr, int line) 
{
    ASTNode* node = newRetStmt(line);
    node->children[0] = expr;
    return node;
}

//--------------------------------
// Expressions 
// -------------------------------
 ASTNode* newId(char *name) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create ID node");
        return node;
    }

    node->label = "ID;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Expr;
    node->kind.exp = Id;
    node->val.sval = name;
    return node;
}

 ASTNode* newIdLine(char *name, int line) 
{
    ASTNode* node = newId(name);
    node->line = line;
    return node;
}

 ASTNode* newTypeId(char* name) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create TypeId node");
        return node;
    }

    node->label = "TypeID;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Expr;
    node->kind.exp = TypeId;
    node->val.sval = name;
    return node;
}

 ASTNode* newTypeIdLine(char* name, int line) 
{
    ASTNode* node = newTypeId(name);
    node->line = line;
    return node;
}

 ASTNode* newIntBasicLit(int value, int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create int literal node");
        return node;
    }

    node->label = "NUM;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Expr;
    node->kind.exp = BasicLit;
    node->val.ival = value;
    node->line = line;
    node->type_name = INT;
    return node;
}

 ASTNode* newStrBasicLit(char* value, int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create string literal node");
        return node;
    }

    node->label = "STR;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Expr;
    node->kind.exp = BasicLit;
    node->val.sval = value;
    node->line = line;
    node->type_name = STR;
    return node;
}

 ASTNode* newUnExp(Oper op, ASTNode* right, int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create UnaryExp node");
        return node;
    }

    node->label = "Unary;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Expr;
    node->kind.exp = UnaryExp;
    node->line = line;
    node->val.op = op;
    node->children[0] = right;
    return node;
}

 ASTNode* newBinExp(Oper op, ASTNode* left, ASTNode* right, int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create BinaryExp node");
        return node;
    }

    node->label = "BinaryOp;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Expr;
    node->kind.exp = BinaryExp; // UnaryExp; // bug?: should it be rather BinaryExp?
    node->line = line;
    node->val.op = op;
    node->children[0] = left;
    node->children[1] = right;
    return node;
}

 ASTNode* newFuncCall(ASTNode* name, ASTNode* args) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create FuncCall node");
        return node;
    }

    node->label = "FuncCall;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Expr;
    node->kind.exp = FuncCall;
    node->children[0] = name;
    node->children[1] = args;
    return node;
}

 ASTNode* newActual(ASTNode* expr) 
{
    
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create Actual node");
        return node;
    }

    node->label = "Actual;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Expr;
    node->kind.exp = Actual; // bug fix: this should be actual not funccall
    node->children[0] = expr;
    return node;
}

 ASTNode* newActuals() 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create Actuals node");
        return node;
    }

    node->label = "Actuals;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Expr;
    node->kind.exp = Actuals;
    return node;
}

//--------------------------------
// Declaratations
// -------------------------------
 ASTNode* newFuncDecl(ASTNode* id, ASTNode* sig, ASTNode* body, int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create FuncDecl node");
        return node;
    }

    node->label = "FuncDecl;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Decl;
    node->kind.decl = FuncDecl;
    node->line = line;
    node->children[0] = id;
    node->children[1] = sig;
    node->children[2] = body;
    return node;
}

 ASTNode* newGlobVarDecl(ASTNode* id, ASTNode* type, int line)
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create VarDecl node");
        return node;
    }

    node->label = "GlobVarDecl;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Decl;
    node->kind.decl = GlobVarDecl;
    node->line = line;
    node->children[0] = id;
    node->children[1] = type;
    return node;
}
 ASTNode* newVarDecl(ASTNode* id, ASTNode* type, int line)
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create VarDecl node");
        return node;
    }

    node->label = "VarDecl;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Decl;
    node->kind.decl = VarDecl;
    node->line = line;
    node->children[0] = id;
    node->children[1] = type;
    return node;
}

 ASTNode* newParamDecl(ASTNode *id, ASTNode* type) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create ParamDecl node");
        return node;
    }

    node->label = "Param;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Decl;
    node->kind.decl = ParamDecl;
    node->children[0] = id;
    node->children[1] = type;
    return node;
}

 ASTNode* newParams() 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create Params node");
        return node;
    }

    node->label = "Params;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Decl;
    node->kind.decl = Params;
    return node;
}

 ASTNode* newSignature(ASTNode* type, ASTNode* params) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create Signature node");
        return node;
    }

    node->label = "Sig;";
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->next = NULL;
    node->node_type = Decl;
    node->kind.decl = Signature;
    node->children[0] = type;
    node->children[1] = params;
    return node;
}



// NOTE: Parameters and actuals are done this way to 
// save memory. Most nodes will not have so many children
// and this prevents having a max number of arguments/params/statements in a block

 void append(ASTNode* stmt1, ASTNode* stmt2)
{
    ASTNode *tmp = stmt1;
    while (tmp->next != NULL) tmp = tmp->next;
    tmp->next = stmt2;
}

 void addActual(ASTNode* parent, ASTNode* actual)
{
    if (parent->children[0] != NULL)
    {
        ASTNode* tmp = parent->children[0];
        while (tmp->next != NULL) tmp = tmp->next;
        tmp->next = actual;
    }
    else 
    {
        parent->children[0] = actual;
    }
}

 void addParam(ASTNode* parent, ASTNode* param)
{
    if (parent->children[0] != NULL)
    {
        ASTNode* tmp = parent->children[0];
        while (tmp->next != NULL) tmp = tmp->next;
        tmp->next = param;
    }
    else 
    {
        parent->children[0] = param;
    }
}

// Place printTree somewhere else in a C file and remove 
 void printTree(ASTNode* tree, FILE* out,int INDENT) 
{
    while (tree != NULL)
    {
        // Print indentation
        fprintf(out, "%*s", INDENT*4, "");

        // Print label first
        fprintf(out, "%s", tree->label); 

        switch (tree->node_type) 
        {
            case Prog:
                break;
            case Stmt:
                switch (tree->kind.stmt) 
                {
                    case IfStmt:
                    case IfElseStmt:
                    case Assn:
                    case BreakStmt:
                    case ReturnStmt:
                        fprintf(out, " Line: %d", tree->line);
                        break;
                    case For:
                    case Block:
                    case Empty:
                    case ExprStmt:
                    case DeclStmt:
                    default:
                        break;
                }
                break;
            case Expr:
                switch (tree->kind.exp) 
                {
                    case Id:
                    case TypeId:
                        fprintf(out, " Value: %s, Line: %d", tree->val.sval, tree->line);
                        break;
                    case BasicLit:
                        if (tree->type_name == INT)
                            fprintf(out, " Value: %d, Line: %d", tree->val.ival, tree->line);
                        else
                            fprintf(out, " Value: %s, Line: %d", tree->val.sval, tree->line);
                        break;
                    case UnaryExp:
                    case BinaryExp:
                        fprintf(out, " %s, Line: %d", opToString(tree->val.op), tree->line);
                        break;
                    case FuncCall:
                    case Actual:
                    case Actuals:
                    default:
                        break;
                }
                break;
            case Decl:
                switch (tree->kind.decl) 
                {
                    case FuncDecl:
                    case VarDecl:
                        fprintf(out, " Line: %d", tree->line);
                        break;
                    case GlobVarDecl:
                        fprintf(out, " Line: %d", tree->line);
                        break;
                    case ParamDecl:
                    case Params:
                    case Signature:
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        fprintf(out, "\n");
        
        INDENT++;
        for (int i = 0; i < MAX_CHILDREN; i++) {
        // if (tree->children[i] != NULL) printf("%*s__child%d\n", (INDENT-1)*4,"",i);
            printTree(tree->children[i], out, INDENT);}
        INDENT--;
        // printf("%*s__next\n", (INDENT)*4,"");
        tree = tree->next;
    }
}


void preTraversal(ASTNode* tree, void (* preFunc)(ASTNode * tree), int (* haltFunc)(ASTNode * tree)) {
    prePostTraversal(tree, preFunc, NULL, haltFunc);
}
void prePostTraversal(ASTNode* tree, void (* preFunc)(ASTNode * tree), void (* postFunc)(ASTNode * tree), int (* haltFunc)(ASTNode * tree)) {
    if (haltFunc != NULL && haltFunc(tree)) return;
    if (preFunc != NULL) preFunc(tree);
    if (tree != NULL) {
        for (int i = 0; i < MAX_CHILDREN; i++) prePostTraversal(tree->children[i], preFunc, postFunc, haltFunc);
        prePostTraversal(tree->next, preFunc, postFunc, haltFunc); // makes post order fixed, but messes up preorder
    }
    if (postFunc != NULL) postFunc(tree);
}
void postTraversal(ASTNode* tree, void (* postFunc)(ASTNode * tree), int (* haltFunc)(ASTNode * tree)) {
    prePostTraversal(tree, NULL, postFunc, haltFunc);
}
