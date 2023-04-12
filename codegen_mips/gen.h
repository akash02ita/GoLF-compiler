#pragma once
#include "astC.h"
#include "semantic.h"
#include "hashmap.h"
#include "stack.h"

void gencode(ASTNode * node, FILE * outcode);
void trav(ASTNode * node);

void allocate(ASTNode * funcnode);
void deallocate(ASTNode * funcnode);

// void applyblock();

// void evalexpr();


// write instruction subroutine
void writei(char * ins);
extern FILE * outcode;



void buildSigTable(ASTNode * node);
void buildLVTable(ASTNode * node);