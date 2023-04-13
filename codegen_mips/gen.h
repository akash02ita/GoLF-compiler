#pragma once
#include "astC.h"
#include "semantic.h"
#include "hashmap.h"
#include "stack.h"

void gencode(ASTNode * node, FILE * outcode);
void trav(ASTNode * node); // traverse thorugh toplevedeclr
void applyBlock(ASTNode * blocknode, char * label, char * retlabel, char * breaklabel);
void applyFunctionCall(ASTNode * funccallnode);
int evalExpression(ASTNode * node, char * truebranchlabel, char * falsebranchlabel);

void allocate(ASTNode * funcnode);
void deallocate(ASTNode * funcnode);

// write instruction subroutine
void writei(char * ins);
extern FILE * outcode;



void buildSigTable(ASTNode * node);
void buildLVTable(ASTNode * node);

void addLV(ASTNode * node);
void applyIntOp(Oper op);
void applyRelOp(Oper op, int type);