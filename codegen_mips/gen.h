#pragma once
#include "astC.h"
#include "semantic.h"
#include "hashmap.h"
#include "stack.h"

void gencode(ASTNode * node, FILE * out);
void trav(ASTNode * node, FILE * out);

void allocate(ASTNode * funcnode, FILE * out);
void deallocate(ASTNode * funcnode, FILE * out);

// void applyblock();

// void evalexpr();


// write instruction subroutine
void writei(FILE * out, char * ins);
