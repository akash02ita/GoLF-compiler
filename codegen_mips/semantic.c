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
    // printf("\t\t===== pass 1: forward ======\n");
    preTraversal(asttree, pass1, NULL);
    closescope(1); // destroy file block (pass2 will remake the file block)
    addFileBlock(); // openscope file block (new and fresh!!)
    // printf("\t\t===== pass 1: reverse ======\n");
    postTraversal(asttree, pass1, NULL); // do not close file from this point as it will be reused in lookup

    // printf("\t\t===== pass 2: prepost ======\n");
    // prePostTraversal(asttree, pass2, pass2post, NULL); // problem: scopes are not closed on exit of function!
    preTraversalPostBeforeNext(asttree, pass2, pass2post, NULL); // solves problem

    pass3(asttree);    
    pass4(asttree);    
}


void addUniverseBlock() {
    // openscope("universe");
    openscope(""); // empty prefix is more helpful so that when getting labelname=functoiname+scope.name, prints, printb remain same
    Scope * universeScope = stackPop(stackstab);
    hashMapInit(universeScope, 1);
    assert (universeScope->capacity > 0);

    // predeclared types
    ScopeValue * typevalue = (ScopeValue *) malloc(sizeof(ScopeValue));
    typevalue->provenience = "";
    typevalue->istype = true; typevalue->isconst = false; typevalue->isfunc = false; typevalue->isid = false;
    hashMapInsert(universeScope, "void", typevalue);
    hashMapInsert(universeScope, "string", typevalue);
    hashMapInsert(universeScope, "int", typevalue);
    hashMapInsert(universeScope, "bool", typevalue);

    ScopeValue * funcvalue = (ScopeValue *) malloc(sizeof(ScopeValue));
    funcvalue->provenience = "";
    funcvalue->istype = false; funcvalue->isconst = false; funcvalue->isfunc = true; funcvalue->isid = false;
    hashMapInsert(universeScope, "prints", funcvalue);
    hashMapInsert(universeScope, "printi", funcvalue);
    hashMapInsert(universeScope, "printb", funcvalue);
    hashMapInsert(universeScope, "printc", funcvalue);
    
    
    ScopeValue * constvalue = (ScopeValue *) malloc(sizeof(ScopeValue));
    constvalue->provenience = "";
    constvalue->istype = false; constvalue->isconst = true; constvalue->isfunc = false; constvalue->isid = false;
    hashMapInsert(universeScope, "true", constvalue);
    hashMapInsert(universeScope, "false", constvalue);
    // predeclared constants
    // ScopeValue universe[] = {
        
    //     // {.istype=false, .isid=false}
    // }

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
        char * name_key = strdup(node->children[0]->val.sval);
        char * type_var = node->children[1]->val.sval;
        // first define var
        if (hashMapFind(currscope, name_key) != NULL) {
            fprintf(stderr, "error: define: `%s` is being redefined at or near line %d\n", name_key, node->children[0]->line);
            exit(EXIT_FAILURE);
        }
        value->isid = true;
        value->type = type_var; // type of identifier
        value->line = node->children[0]->line;
        // value->provenience = "_file"; // global scope in file block
        value->provenience = getStackProvenience(); // same as above
        
        hashMapInsert(currscope, name_key, value);
        // then check/lookup type of var being valid
        ScopeValue * entryfound = lookup(type_var);
        if (entryfound->istype != true) {
            fprintf(stderr, "error: define: %s's type `%s` is not a type, at or near line %d\n", name_key, type_var, node->children[0]->line);
            exit(EXIT_FAILURE);
        }
        node->sym = (void *) value;
    }
    else if (node->node_type == Decl && node->kind.decl == VarDecl) {
        char * name_key = strdup(node->children[0]->val.sval);
        char * type_var = node->children[1]->val.sval;
        // first check/lookup type of var being valid
        ScopeValue * entryfound = lookup(type_var);
        if (entryfound->istype != true) {
            fprintf(stderr, "error: define: %s's type `%s` is not a type, at or near line %d\n", name_key, type_var, node->children[0]->line);
            exit(EXIT_FAILURE);
        }

        // then define var
        if (hashMapFind(currscope, name_key) != NULL) {
            fprintf(stderr, "error: define: `%s` is being redefined at or near line %d\n", name_key, node->children[0]->line);
            exit(EXIT_FAILURE);
        }
        value->isid = true;
        value->type = type_var; // type of identifier
        value->line = node->children[0]->line;

        value->provenience = getStackProvenience(); // provenience requires to pop all scopes and add names!
        // // printf("the net provenience is %s\n", value->provenience); getchar();

        hashMapInsert(currscope, name_key, value);
        node->sym = (void *) value;
    }
    else if (node->node_type == Decl && node->kind.decl == FuncDecl) {
        value->isfunc = true;
        value->line = node->children[0]->line;
        // value->provenience = "_file"; // global scope in file block
        // only "main" remains without any prefix
        
        char * functionname = node->children[0]->val.sval;
        if (strcmp(functionname, "main") != 0) value->provenience = getStackProvenience();
        else { value->provenience = strdup(""); }

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

        value->sig = (char *) malloc(1000*sizeof(char));
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
        // also put the same for type, as it will not affect logic (but if using value.type instea might be easier in some parts)
        value->type = value->rettype;
        hashMapInsert(currscope, functionname, value);
        node->sym = (void *) value;
    }
    else if (node->node_type == Decl && node->kind.decl == Signature) {
        // over here assume caller has already opened the scope for the function!
        // so curretscope is scope of function body!!!!!!!!!
        // by this point the types are valid and thus they only need to be defined
        ASTNode * param = node->children[1]->children[0];
        while (param != NULL) {
            char * paramname = param->children[0]->val.sval;
            char * paramtype = param->children[1]->val.sval;
            ScopeValue * paramvalue = (ScopeValue *) malloc(sizeof(ScopeValue));
            paramvalue->isid = true; paramvalue->isconst = false; paramvalue->isfunc = false; paramvalue->istype = false;
            paramvalue->type = paramtype; // int string bool
            
            hashMapInsert(currscope, paramname, paramvalue); // define parameter id inside functionbody scope
            paramvalue->provenience = getStackProvenience();
            param->sym = (void *) paramvalue;
            param->children[0]->sym = (void *) paramvalue;
            // param->children[1]->sym = (void *) paramvalue;
            param = param->next;
        }
        
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
        // fprintf(stdout, "pass1 detected global var %s\n", asttree->children[0]->val.sval);
        // do define of varname (function define handles type verification as well automatically)
        define(asttree);
    }
    if (asttree->node_type == Decl && asttree->kind.decl == FuncDecl) {
        // fprintf(stdout, "pass1 detected function %s\n", asttree->children[0]->val.sval);
        // do define of function ( define handles signature type verification as well automatically)
        define(asttree);
    }
}

int nested_block = 0;
void pass2(ASTNode * asttree) {
    ASTNode * node = asttree;
    assert (node != NULL);
    if (node->node_type == Decl && node->kind.decl == FuncDecl) {
        const char * functionname = node->children[0]->val.sval;
        // printf("Opening scope for functionname %s\n", functionname);
        openscope(functionname);
    }
    else if (node->node_type == Stmt && node->kind.decl == IfStmt) {
        // printf("Opening scope for if statement\n");
        openscope("if");
    }
    else if (node->node_type == Stmt && node->kind.decl == IfElseStmt) {
        // printf("Opening scope for ifelse statement\n");
        openscope("ifelse");
    }
    else if (node->node_type == Stmt && node->kind.decl == Block) {
        if (nested_block > 0) { // function starts with a body as the 1st block. but must prevent that to openscope in that case
            // printf("Opening scope for Block statement %d\n", nested_block);
            openscope("block");
        }
        nested_block++;
    }
    else if (node->node_type == Decl && node->kind.decl == Signature) {
        // printf("\tPass2 found signature %s\n", node->children[0]->val.sval);
        define(node); // define the signature of parameters in body
    }
    else if (node->node_type == Decl && node->kind.decl == VarDecl) {
        // printf("\tPass2 found local var %s\n", node->children[0]->val.sval);
        define(node);
    }
    else if (nested_block > 0 && node->node_type == Expr && node->kind.exp == Id) {
        // identifier can only be inside the function body!
        // lookup identifier or variable and is of type id
        char * idname = node->val.sval;
        /* 
        this part of semantic check is not quite correct as a functionname is an id (not isid can be false)
        if (lookup(idname)->isid != true) {
            fprintf(stderr, "error: pass2: `%s` is not a variable at or near line %d\n", idname, node->line);
            exit(EXIT_FAILURE);
        }
        */
        node->sym = (void *) lookup(idname);
    }
    else if (node->node_type == Stmt && node->kind.stmt == Assn) {
        // assert that left side of = is an identifier
        ASTNode * lhs = node->children[0];
        if (lhs == NULL) {fprintf(stderr, "error: pass2: assignment lhs is NULL\n"); exit(EXIT_FAILURE); }
        if (lhs->node_type != Expr || lhs->kind.exp != Id) {fprintf(stderr, "error: pass2: lhs of assignment is not an identifier at or near line %d\n", node->line); exit(EXIT_FAILURE); }
        char * idname = lhs->val.sval;
        if (lookup(idname)->isid != true) {
            fprintf(stderr, "error: pass2: `%s` is not a variable at or near line %d\n", idname, node->line);
            exit(EXIT_FAILURE);
        }
    }
    else if (node->node_type == Expr && node->kind.exp == FuncCall) {
        char * functionname = node->children[0]->val.sval;
        // verify function exists and is a type of function
        if (lookup(functionname)->isfunc != true) {
            fprintf(stderr, "error: pass2: `%s` is not a function at or near line %d\n", functionname, node->line);
            exit(EXIT_FAILURE);
        }
        node->sym = (void *) lookup(functionname);
    }
    // DONE: if, for statement to check (check if these block allow something)
    // if  ---> new scope
    // for  ----> new scope
    // assignment --> lookup use case ---> just use id node (as long as block is inside functionbody)
    // expressions --> lookup use cases? ---> just use id checker (as long as lock is inside function body)

}
void pass2post(ASTNode * node) {
    if (node->node_type == Decl && node->kind.decl == FuncDecl) {
        const char * functionname = node->children[0]->val.sval;
        // printf("Closing scope for functionname %s\n", functionname);
        closescope(0); // close function body scope without losing data!
    }
    else if (node->node_type == Stmt && node->kind.decl == IfStmt) {
        // printf("Closing scope for if statement\n");
        closescope(0);
    }
    else if (node->node_type == Stmt && node->kind.decl == IfElseStmt) {
        // printf("Closing scope for ifelse statement\n");
        closescope(0);
    }
    else if (node->node_type == Stmt && node->kind.decl == Block) {
        nested_block--;
        if (nested_block > 0)
        {
            // printf("Closing scope for Block statement %d\n", nested_block);
            // printTree(node, stdout, 4);
            closescope(0);
        }
    }
}

void pass3(ASTNode * asttree) {
    // do type checking: not yet required for code gen
}
void pass4(ASTNode * asttree) {
    // do remaining checks: not yet required for code gen
}

void pass5(ASTNode * asttree) {
}

void pass6(ASTNode * asttree) {
}



char * getStackProvenience() {
    int BUFF_SIZE = 1;
    char * provenience = (char *) malloc(BUFF_SIZE*sizeof(char));
    provenience[0] = '\0';


    int before = stackstab->length;
    struct Stack * temp = (struct Stack *) malloc(sizeof(struct Stack));
    assert (temp != NULL);
    stackInit(temp, 1);
    while (stackstab->length > 0) stackPush(temp, stackPop(stackstab));
    while (temp->length > 0) {
        Scope * currentscope = (Scope *) stackPop(temp);
        
        // provenicne prefix = prefi + _ + scope.name
        if (currentscope->name != NULL && strcmp(currentscope->name, "") != 0) // universe has empty value. As convention, do not append anyting if scopename is empty
        {
            BUFF_SIZE = mystrcat(&provenience, "_", BUFF_SIZE); // from lex.h
            BUFF_SIZE = mystrcat(&provenience, currentscope->name, BUFF_SIZE); // from lex.h
        }

        stackPush(stackstab, currentscope);

    }
    assert (stackstab->length == before);
    free(temp);

    return provenience;
}