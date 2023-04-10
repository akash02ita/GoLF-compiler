#include "semantic.h"
#include <assert.h>
#include <string.h>

struct Stack * stackstab;


void semantic(ASTNode * asttree) { // should annotate tree passed by ref
    stackstab = (struct Stack * ) malloc(sizeof(struct Stack));
    if (stackstab == NULL) {
        fprintf(stderr, "error: stackstab: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    stackInit(stackstab, 1);
    // openscope of universe block (DO NOT DESTORY IT)
    addUniverseBlock();

    // openscope file block
    addFileBlock();
    printf("\t\t===== pass 1: forward ======\n");
    preTraversal(asttree, pass1, NULL);
    closescope(1); // destroy file block (pass2 will remake the file block)
    addFileBlock(); // openscope file block (new and fresh!!)
    printf("\t\t===== pass 1: reverse ======\n");
    postTraversal(asttree, pass1, NULL);


    pass2(asttree);    
    // no need to close as it will be used
    pass3(asttree);    
    pass4(asttree);    
}


void addUniverseBlock() {
    openscope("universe");
    Scope * universeScope = stackPop(stackstab);
    hashMapInit(universeScope, 1);
    assert (universeScope->capacity > 0);

    // predeclared types
    ScopeValue * typevalue = (ScopeValue *) malloc(sizeof(ScopeValue));
    typevalue->istype = true; typevalue->isconst = false; typevalue->isfunc = false; typevalue->isid = false;
    hashMapInsert(universeScope, "void", typevalue);
    hashMapInsert(universeScope, "string", typevalue);
    hashMapInsert(universeScope, "int", typevalue);
    hashMapInsert(universeScope, "bool", typevalue);
    
    // predeclared constants

    // predeclared functions
    
    


    stackPush(stackstab, universeScope);
}

void addFileBlock() {
    openscope("file");
}

void openscope(const char * scopename) {
    Scope * newScope = (Scope *) malloc(sizeof(Scope));
    assert (newScope != NULL);

    hashMapInit(newScope, 1);
    assert (newScope->capacity > 0); 

    if (scopename != NULL) newScope->name = strdup(scopename);

    int before = stackstab->length;
    stackPush(stackstab, (void *) newScope);
    assert (stackstab->length == before + 1);
}

void closescope(bool freeflag) {
    int before = stackstab->length;
    Scope * currsscope = stackPop(stackstab);
    if (currsscope == NULL ) {
        fprintf(stderr, "error: currscope: is null\n");
        exit(EXIT_FAILURE);
    }
    if (freeflag) {
        // TODO: free single thing inside element scope as well
        free(currsscope); // may not want to closescope when doing code generation
    }
    assert (stackstab->length == before - 1);
}


void define(ASTNode * node) {
    assert (node != NULL);

    Scope * currscope = (Scope *) stackPop(stackstab);
    assert (currscope != NULL);
    stackPush(stackstab, (void *) currscope);
    // if (currscope->name != NULL) fprintf(stdout, "debug: define: currscope name is {%s}\n", currscope->name);

    ScopeValue * value = (ScopeValue *) malloc(sizeof(ScopeValue));
    assert (value != NULL); 
    value->isconst = false; value->isfunc = false; value->isid = false; value->istype = false;

    if (node->node_type == Decl && node->kind.decl == GlobVarDecl) {
        // first define var
        char * name_key = strdup(node->children[0]->val.sval);
        if (hashMapFind(currscope, name_key) != NULL) {
            fprintf(stderr, "error: define: `%s` is being redefined at or near line %d\n", name_key, node->children[0]->line);
            exit(EXIT_FAILURE);
        }
        value->isid = true;
        value->line = node->children[0]->line;
        value->provenience = "_file"; // global scope in file block
        hashMapInsert(currscope, name_key, value);
        // then check/lookup type of var being valid
        char * type_var = node->children[1]->val.sval;
        ScopeValue * entryfound = lookup(type_var);
        if (entryfound->istype != true) {
            fprintf(stderr, "error: define: %s's type `%s` is not a type, at or near line %d\n", name_key, type_var, node->children[0]->line);
            exit(EXIT_FAILURE);
        }
    }
    else if (node->node_type == Decl && node->kind.decl == VarDecl) {
        // first check/lookup type of var being valid
        // then define var
    }
    else if (node->node_type == Decl && node->kind.decl == FuncDecl) {
        value->isfunc = true;
        value->line = node->children[0]->line;
        value->provenience = "_file"; // global scope in file block
        char * functionname = node->children[0]->val.sval;

        // verify signature proper types (so that every parameter has valid type)
        ASTNode * sig = node->children[1];
        char * rettype = sig->children[0]->val.sval;
        if (strcmp(rettype, "") == 0) rettype = "void";
        if (lookup(rettype)->istype != true) {
            fprintf(stderr, "error: define: function %s's return type `%s` is not a type, at or near line %d", functionname, rettype, node->line);
            exit(EXIT_FAILURE);
        }

        Scope * tempParamScope = (Scope *) malloc(sizeof(Scope)); // do not openscope! Just check parameters being not redefined
        assert (tempParamScope != NULL);
        hashMapInit(tempParamScope, 1);

        value->sig = (char *) malloc(sizeof(1000));
        value->sig[0] = '\0';
        ASTNode * param = sig->children[1]->children[0];
        while (param != NULL) {
            char * paramname = param->children[0]->val.sval;
            char * paramtype = param->children[1]->val.sval;
            if (lookup(paramtype)->istype != true) {
                fprintf(stderr, "error: define: function %s param %s's type `%s  is not a type, at or near line %d\n", functionname, paramname, paramtype, node->line);
                exit(EXIT_FAILURE);
            }
            if (hashMapFind(tempParamScope, paramname) != NULL) {
                fprintf(stderr, "error: define: function %s's param %s is redefined, at or near line %d\n", functionname, paramname, node->line);
                exit(EXIT_FAILURE);
            }
            hashMapInsert(tempParamScope, paramname, "dummy-non-null-value");
            param = param->next;

            // update value.sig
            if (strlen(value->sig) > 0) strcat(value->sig, " ");
            strcat(value->sig, paramtype); // assume value.sig has enogh buffer size (can realloc and copy, but not needed for now)
        }

        free(tempParamScope->buffer);
        free(tempParamScope);

        if (strcmp(value->sig, "") == 0) value->sig = "void";

        // define the function in FILE BLOCK!!!
        if (hashMapFind(currscope, functionname) != NULL) {
            fprintf(stderr, "error: define: `%s` is redefined at or near line %d\n", functionname, node->line);
            exit(EXIT_FAILURE);
        }
        hashMapInsert(currscope, functionname, value);
    }
    else { fprintf(stderr, "error: define: invalid node passed\n"); exit(EXIT_FAILURE); }


}

ScopeValue * lookup(char * name) {
    int before = stackstab->length;
    struct Stack * temp = (struct Stack *) malloc(sizeof(struct Stack));
    stackInit(temp, 1);
    assert (temp->capacity > 0);
    if (temp == NULL) { fprintf(stderr, "error: lookup: temp: malloc is null\n"); exit(EXIT_FAILURE); }
    ScopeValue * entryfound = NULL;
    while (stackstab->length > 0) {
        Scope * currentscope = (Scope *) stackPop(stackstab);
        // check if inside scope or not
        entryfound = (ScopeValue *) hashMapFind(currentscope, name);
        stackPush(temp, currentscope);
        if (entryfound != NULL) break;
    }
    while (temp->length > 0) stackPush(stackstab, stackPop(temp));
    assert (stackstab->length == before);
    free(temp);

    if (entryfound == NULL) { fprintf(stderr, "error: lookup: cannot find `%s` defined\n", name); exit(EXIT_FAILURE); }
    return entryfound;
}


/* optional: to halt function earlier for efficiency
int haltpass1(ASTNode * asttree) {
    if (asttree == NULL) return true;
    if (asttree->node_type == Decl && asttree->kind.decl == GlobVarDecl) return false;
    return false;
}
*/
void pass1(ASTNode * asttree) {
    if (asttree == NULL) return;
    if (asttree->node_type == Decl && asttree->kind.decl == GlobVarDecl) {
        fprintf(stdout, "pass1 detected global var %s\n", asttree->children[0]->val.sval);
        // do define of varname (function define handles type verification as well automatically)
        define(asttree);
    }
    if (asttree->node_type == Decl && asttree->kind.decl == FuncDecl) {
        fprintf(stdout, "pass1 detected function %s\n", asttree->children[0]->val.sval);
        // do define of function ( define handles signature type verification as well automatically)
        define(asttree);
    }
}
void pass2(ASTNode * asttree) {
    // do pass3 or something else here?
}
void pass3(ASTNode * asttree) {
    // check function decl (only in file block)
}
void pass4(ASTNode * asttree) {
    // do local var define and lookup check
}

void pass5(ASTNode * asttree) {
    // do type checking: not yet required for code gen
}

void pass6(ASTNode * asttree) {
    // do remaining checks: not yet required for code gen
}