#include "gen.h"

// FILE * outputcode;

void gencode(ASTNode * tree, FILE * out) {
    trav(tree, out);
}
void trav(ASTNode * node, FILE * out) {
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
            if (strcmp(type_var, "bool") == 0) {
                fprintf(out, ".word 0");
            }
            if (strcmp(type_var, "string") == 0) {
                fprintf(out, ".word $string_empty");
            }
            fprintf(out, "\n");

            // global vars in future will be accessed from label NOT table hashmap!
            // if "label" is not in hashmap then it will be a global var!
        }

        if (node->node_type == Decl && node->kind.decl == FuncDecl) {
            struct HashMap table;
            hashMapInit(&table, 1);
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
            allocate(node, out);

            char * retlabel = label;
            buffsize = mystrcat(&retlabel, "_ret", buffsize);
            printf("return label is %s:\n", retlabel);

            // step3: write code for function block


            fprintf(out, "%s\n", retlabel);
            // step4: write code to deallocate stack frame
            deallocate(node, out);

            free(table.buffer);
        }

        node = node->next;
    }
    
}

void allocate(ASTNode * funcnode, FILE * out) {
    writei(out, "addi $sp, $sp, -4");
    writei(out, "sw $fp, 0($sp)"); // save old fp
    writei(out, "addi $sp, $sp, -4");
    writei(out, "sw $ra, 0($sp)"); // save old ra
    // frame point at bottom of stack frame (+ offset for args, - offset for LVs)
    writei(out, "addi $fp, 8($sp)");

    // now allocate LVs (local vars)
}
void deallocate(ASTNode * funcnode, FILE * out) {
    // deallocate LVs

    // restore fp, lr
    writei(out, "lw $ra, 0($sp)");
    writei(out, "addi $sp, $sp, 4");
    writei(out, "lw $fp, 0($sp)");
    writei(out, "addi $sp, $sp, 4");
    

    // write return code
    writei(out, "jr $ra");
    

}




void writei(FILE * out, char * ins) {
    fprintf(out, "\t%s\n", ins);
}