#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "astC.h"
#include "stack.h"
#include "hashmap.h"

#ifndef bool
#define bool int
#endif
#ifndef false
#define false 0
#endif
#ifndef true
#define true 1
#endif

void semantic(ASTNode * asttree);
extern struct Stack * stackstab;

// a hashamp will be a scope. but each element will be ScopeElement
typedef struct HashMap Scope;
typedef struct {
    bool istype; // key is a type (int, bool, string)

    bool isid; // key is an id (var name)
    char * type; // type of identifier key: string int bool
    
    bool isfunc; // key is a function (function name)
    char * sig; // signature of function: globvar var f(void) f(int) f(bool) f(int, bool, string) .... or much better if just "void", "int", "int bool", "string int bool bool", which is easier to split or parse
    char * rettype; // return type of function

    bool isconst; // key is a constant type (true, false)
    char * constval;

    int line; // not sure if needed

    // provenience can be a string that tells the prefix of location of block
    // if global then provenience is "_file"
    // if inside a for statement, which is inside a functionname then provenience is "_file_functionname_if"
    //    provenience can be set by using parent provenience + "_" + the name of currentscope (see struct Hashmap name field)
    // this provenience tag, can easily help to create ''unique'' or reusable labels as simple as "name" + "provenience", where name is the key of this scopevalue in hashmap
    char * provenience; // for code generation to make it easier to create labels
} ScopeValue;



// -------------------------------------------------------------------------------------------------
/* utility methods */
void openscope(const char * scopename);
void closescope(bool freeflag);

void addUniverseBlock();
void addFileBlock();

// typedef enum { _FuncDecl, _GlobVarDecl, _VarDecl } DEFINE_USE_MODE;
// void define(char * name, int line, DEFINE_USE_MODE mode);
// void lookup(char * name, DEFINE_USE_MODE mode);
void define(ASTNode * node);
ScopeValue * lookup(char * name);

void pass1(ASTNode * asttree);
void pass2(ASTNode * asttree);
void pass3(ASTNode * asttree);
void pass4(ASTNode * asttree);


#endif