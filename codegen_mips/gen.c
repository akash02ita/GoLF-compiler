#include "gen.h"
#include <assert.h>

FILE * out;
struct HashMap * table;
int sigcounter = 0;
int lvcounter = 0;

void gencode(ASTNode * tree, FILE * outcode) {
    out = outcode; // set the output file to write
    trav(tree);
    // append template assembly code with predefined stuff
    FILE * template = fopen("TEMPLATE.s", "r");
    assert (template != NULL);
    char ch;
    while ((ch = fgetc(template)) != EOF)
        fputc(ch, out);
    
}
void trav(ASTNode * node) {
    while (node != NULL)
    {
        if (node->node_type == Decl && node->kind.decl == GlobVarDecl) {
            char * name_key = strdup(node->children[0]->val.sval);
            char * type_var = node->children[1]->val.sval;
            ScopeValue * value = (ScopeValue *) node->sym;
            int buffsize = 1;
            char * label = (char *) malloc(buffsize); label[0] = '\0';
            buffsize = mystrcat(&label, name_key, buffsize);
            buffsize = mystrcat(&label, value->provenience, buffsize);
            // printf("label name is %s\n", label);

            fprintf(out, ".data\n.align 2\n"); fprintf(out, "%s:\t", label);
            if (strcmp(type_var, "int") == 0) {
                fprintf(out, ".word 0");
            }
            else if (strcmp(type_var, "bool") == 0) {
                fprintf(out, ".word 0");
            }
            else if (strcmp(type_var, "string") == 0) {
                fprintf(out, ".word $string_empty");
            } else { assert(0); }
            fprintf(out, "\n");

            // global vars in future will be accessed from label NOT table hashmap!
            // if "label" is not in hashmap then it will be a global var!
        }

        if (node->node_type == Decl && node->kind.decl == FuncDecl) {
            // struct HashMap table;
            table = (struct HashMap *) malloc(sizeof(struct HashMap));
            hashMapInit(table, 1);
            // step1: assign positive offsets relative to fp to access args
            // step2: assign negative offsets relative to fp to access LVs

            // get functionname unique label
            char * functionname = node->children[0]->val.sval;
            int buffsize = 1;
            ScopeValue * value = (ScopeValue *) node->sym;
            char * label = (char *) malloc(buffsize); label[0] = '\0';
            buffsize = mystrcat(&label, functionname, buffsize);
            buffsize = mystrcat(&label, value->provenience, buffsize);
            printf("label name is %s\n", label);

            // step1: add LABELNAMe of function
            fprintf(out, ".text\n"); fprintf(out, "%s:\n", functionname);
            // step2: write code for STACK frame allocation
            sigcounter = 0; lvcounter = -8-4;
            preTraversalPostBeforeNext(node->children[1], buildSigTable, NULL, NULL);
            preTraversalPostBeforeNext(node->children[2], buildLVTable, NULL, NULL);
            allocate(node);

            char * retlabel = label;
            buffsize = mystrcat(&retlabel, "_ret", buffsize);
            printf("return label is %s:\n", retlabel);

            // step3: write code for function block
            


            fprintf(out, "%s:\n", retlabel);
            // step4: write code to deallocate stack frame
            deallocate(node);

            free(table->buffer);
            free(table);

        }

        node = node->next;
    }
    
}

void allocate(ASTNode * funcnode) {
    fprintf(out, "\t# Save old fp and lr\n");
    writei("addi $sp, $sp, -4");
    writei("sw $fp, 0($sp)"); // save old fp
    writei("addi $sp, $sp, -4");
    writei("sw $ra, 0($sp)"); // save old ra
    // frame point at bottom of stack frame (+ offset for args, - offset for LVs)
    writei("addi $fp, $sp, 8");

    // now allocate LVs (local vars)
    lvcounter = -8-4; // jump fp, lr copies and -4 to use the appropriate top
    preTraversalPostBeforeNext(funcnode->children[2], addLV, NULL, NULL);
    printf("Allocations completd ------------------\n");
    
}

void buildSigTable(ASTNode * node) {
    // loop thogh each param
    if (node->node_type == Decl && node->kind.decl == ParamDecl) {
        char * parname = node->children[0]->val.sval;
        ScopeValue * value = (ScopeValue *) node->sym;
        // printf("parname is %s and provenience is %s\n", parname, value->provenience);
        int size = 1; char * unique_name = (char *) malloc(size); unique_name[0] = '\0';
        size = mystrcat(&unique_name, parname, size);
        size = mystrcat(&unique_name, value->provenience, size); 
        int * offset = malloc(sizeof(int));
        *offset = sigcounter;
        // printf("buildtableSig: Param unique name %s offset %d\n", unique_name, *offset); getchar();
        sigcounter += 4;
    }
}

void buildLVTable(ASTNode * node) {
    // need to traversal on functionblock
    if (node->node_type == Decl && node->kind.decl == VarDecl) {
        char * varname = node->children[0]->val.sval;
        ScopeValue * value = (ScopeValue *) node->sym;
        int size = 1; char * unique_name = (char *) malloc(size); unique_name[0] = '\0';
        size = mystrcat(&unique_name, varname, size);
        size = mystrcat(&unique_name, value->provenience, size);
        int * offset = malloc(sizeof(int));
        *offset = lvcounter;
        // printf("buildtable: Var decl unique name %s offset %d\n", unique_name, *offset); getchar();
        hashMapInsert(table, unique_name, (void *) offset);
        lvcounter -= 4;        
    }
}


void addLV(ASTNode * node) {
    // if node is a var declarataion
    
    // get name
    // get provenience

    // generate unique_name = name + provenience
    // IF NOT ALREADY DONE: insert in table (BEST TO DO IT HERE)
    
    // add memory on sp
    /*
        case "int"
            sw 0
        case "bool"
            sw 0
        case "string"
            la t0, $string_empty
            sw t0, 0($sp) // save address of label
    */
    // need to traversal on functionblock
    if (node->node_type == Decl && node->kind.decl == VarDecl) {
        char * varname = node->children[0]->val.sval;
        ScopeValue * value = (ScopeValue *) node->sym;
        int size = 1; char * unique_name = (char *) malloc(size); unique_name[0] = '\0';
        size = mystrcat(&unique_name, varname, size);
        size = mystrcat(&unique_name, value->provenience, size);
        int * offset = (int *) hashMapFind(table, unique_name); // not needed
        assert (*offset == lvcounter);
        lvcounter -= 4;

        char * type_var = node->children[1]->val.sval;
        // printf("Variable %s is of type %s\n", unique_name, type_var); getchar();
        fprintf(out, "\t# Alloc %s\n", unique_name);
        writei("addi $sp, $sp, -4  # add memory for lv"); 
        if (strcmp(type_var, "int") == 0) {
            writei("sw $zero, 0($sp)");
        }
        else if (strcmp(type_var, "bool") == 0) {
            writei("sw $zero, 0($sp)");
        }
        else if (strcmp(type_var, "string") == 0) {
            writei("la $t0, $string_empty");
            writei("sw $t0, 0($sp)"); // variable will store address of label always!
        } else { assert(0); }
    }
}

void removeLV(ASTNode * node) {
    /*
        if node is var decl then

        add sp, sp, 4     // deallocate memory
    */
   if (node->node_type == Decl && node->kind.decl == VarDecl) {
        char * varname = node->children[0]->val.sval;
        ScopeValue * value = (ScopeValue *) node->sym;
        int size = 1; char * unique_name = (char *) malloc(size); unique_name[0] = '\0';
        size = mystrcat(&unique_name, varname, size);
        size = mystrcat(&unique_name, value->provenience, size);
        int * offset = (int *) hashMapFind(table, unique_name); // not needed
        assert (*offset == lvcounter);
        lvcounter -= 4;
        fprintf(out, "\t# Dealloc %s\n", unique_name);
        writei("add $sp, $sp, 4"); // just keep deallocating
   }
}


void deallocate(ASTNode * funcnode) {
    // deallocate LVs
    lvcounter = -8-4;
    preTraversalPostBeforeNext(funcnode->children[2], removeLV, NULL, NULL);

    // restore fp, lr
    fprintf(out, "\t# Restore old fp and lr\n");
    writei("lw $ra, 0($sp)");
    writei("addi $sp, $sp, 4");
    writei("lw $fp, 0($sp)");
    writei("addi $sp, $sp, 4");
    

    // write return code
    fprintf(out, "\t# return to caller\n");
    writei("jr $ra");
    

}




void writei(char * ins) {
    fprintf(out, "\t%s\n", ins);
}