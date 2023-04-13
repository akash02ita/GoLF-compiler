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
    fclose(template);
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
            // printf("label name is %s\n", label); getchar();

            // step1: add LABELNAMe of function
            fprintf(out, ".text\n"); fprintf(out, "%s:\n", label);
            // step2: write code for STACK frame allocation
            sigcounter = 0; lvcounter = -8-4;
            preTraversalPostBeforeNext(node->children[1], buildSigTable, NULL, NULL);
            preTraversalPostBeforeNext(node->children[2], buildLVTable, NULL, NULL);
            allocate(node);

            char * retlabel = strdup(label);
            buffsize = mystrcat(&retlabel, "_ret", buffsize);
            // printf("return label is %s:\n", retlabel);

            // step3: write code for function block
            applyBlock(node->children[2], label, retlabel, NULL);


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
    // printf("Allocations completd ------------------\n");
    // fprintf(stderr, "Allocations completd ------------------\n");
    
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
        hashMapInsert(table, unique_name, (void *) offset);
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

int ifcounter = 0;
int forcounter = 0;
char * generateLabel(char * prefix, int counter) {
    char * res = (char *) malloc(1000); res[0] = '\0'; // assume it is enough long
    sprintf(res, "%s_%d", prefix, counter);
    assert( strlen(res) < 1000 ); // verify did not overflow memory
    return res;
}
void applyBlock(ASTNode * blocknode, char * label, char * retlabel, char * breaklabel) {
    if (blocknode == NULL) return;
    if (blocknode->node_type == Stmt) {
        switch (blocknode->kind.stmt)
        {
        case BreakStmt: {
            // write instruction to branch to breaklabel
            fprintf(out, "\tj %s\n", breaklabel);
            break;
        }
        case ReturnStmt: {
            // if return is non-void, set $v0 to appropriate return value
            if (blocknode->children[0] != NULL) {
                evalExpression(blocknode->children[0], NULL, NULL);
                // evalExpression sets $t0 to result of expression
                writei("move $v0, $t0");
            }
            // write instruction to branch to breaklabel
            fprintf(out, "\tj %s\n", retlabel);
            break;
        }
        case Block: {
            // mystrcat(&label, "_block", strlen(label))
            applyBlock(blocknode->children[0], NULL, retlabel, breaklabel);
            break;
        }
        case IfStmt: // will behave same way as ifelse, but with else children = null
        case IfElseStmt: {
            char * bodylabel = generateLabel("ifbody", ifcounter);
            char * elselabel = generateLabel("ifelse", ifcounter);
            ifcounter++;

            // write code for condition checking
            evalExpression(blocknode->children[0], bodylabel, elselabel);
            // write code for body of if
            fprintf(out, "%s:\n", bodylabel);
            applyBlock(blocknode->children[1], NULL, retlabel, breaklabel);
            // write code for else of if
            fprintf(out, "%s:\n", elselabel);
            applyBlock(blocknode->children[2], NULL, retlabel, breaklabel);
            break;
        }
        case For: {
            // new break label!!!
            char * pretestlabel = generateLabel("forpretest", forcounter);
            char * looplabel = generateLabel("forloop", forcounter);
            char * exitlooplabel = generateLabel("forexitloop", forcounter);
            forcounter++;

            fprintf(out, "%s:\n", pretestlabel); // pretest
            evalExpression(blocknode->children[0], looplabel, exitlooplabel); // write code for condition of for loop

            // loop of for
            fprintf(out, "%s:\n", looplabel);
            // write code for's block, with new breaklabel = exitlooplabel
            applyBlock(blocknode->children[1], NULL, retlabel, exitlooplabel);

            fprintf(out, "\tj %s\n", pretestlabel); // writei

            // exit out for label
            fprintf(out, "%s:\n", exitlooplabel);
            break;
        }
        case Assn: {
            // left side must be indentifier
            // right side can be an expression
                // eval expression without any true, false branches
            ASTNode * lhs = blocknode->children[0];
            char * varname = lhs->val.sval;
            ScopeValue * value = (ScopeValue *) lhs->sym;
            char * vartype = value->type;

            int size = 1; char * unique_name = (char *) malloc(size); unique_name[0] = '\0';
            size = mystrcat(&unique_name, varname, size);
            size = mystrcat(&unique_name, value->provenience, size);

            int * offset = hashMapFind(table, unique_name);
            
            // fprintf(stderr, "good news! Found assignment for %s of type %s with offset $fp+ %d\n", varname, vartype, *offset); getchar();

            ASTNode * rhs = blocknode->children[1];
            // $t0 will have result of riht hand side expression
            evalExpression(rhs, NULL, NULL);
            // write code for assignment
            if (offset == NULL) // implies GLOBAL_VAR
            {
                fprintf(out, "\tla $t1, %s\n", unique_name);
                fprintf(out, "\tsw $t0, 0($t1) # assignment glob var %s\n", varname);
            }
            else { // local variable
                fprintf(out, "\tsw $t0, %d($fp) # assignment for %s %s\n", *offset, varname, vartype);
            }

            break;
        }
        case ExprStmt: {
            // only FuncCall must be allowed
            // rest is ignored or can be optional (whichever is easier)
                // like 1+2, var+var2 sitting there for nothing. it does nothing to the actual logic of code. So they can be ignored
            ASTNode * expr = blocknode->children[0];
            assert (expr != NULL);
            if (expr->node_type == Expr && expr->kind.stmt == FuncCall) {
                // getchar();
                applyFunctionCall(expr);
            }
            break;
        }
        default:
            // empty statements are skipped: no purpose in code
            // declarations are skipped: allocation already done
            break;
        }
    }
    applyBlock(blocknode->next, label, retlabel, breaklabel); // do the next as well.

}

void applyFunctionCall(ASTNode * funccallnode) {
    assert (funccallnode != NULL);
    assert (funccallnode->node_type == Expr && funccallnode->kind.stmt == FuncCall);
    char * funcname = funccallnode->children[0]->val.sval;
    int argnum = 0;
    ASTNode * actual = funccallnode->children[1]->children[0];
    while (actual != NULL) {
        evalExpression(actual, NULL, NULL);
        argnum++;
        fprintf(out, "\t# Adding arg #%d in functioncall %s()\n", argnum, funcname);
        writei("addi $sp, $sp, -4");
        writei("sw $t0, 0($sp)");

        actual = actual->next;
    }
    ScopeValue * value = (ScopeValue *) funccallnode->sym;
    assert (value != NULL);
    int buffsize = 1;
    char * label = (char *) malloc(buffsize); label[0] = '\0';
    buffsize = mystrcat(&label, funcname, buffsize);
    buffsize = mystrcat(&label, value->provenience, buffsize);
    // printf("label name is %s\n", label); getchar();
    fprintf(out, "\tjal %s\n", label);

    // do not forget to also deallocate functoin stack memory!
    actual = funccallnode->children[1]->children[0];
    while (actual != NULL) {
        argnum++;
        fprintf(out, "\t# Removing arg #%d in functioncall %s()\n", argnum, funcname);
        writei("addi $sp, $sp, 4");
        actual = actual->next;
    }

}

int stringcounter = 0;
char * addString(char * buff) {
    char * label = generateLabel("string", stringcounter);
    fprintf(out,".data\n.align 2\n%s: .byte ", label);
    while (*buff) {
        char byte;
        if (*buff == '\\') {
            // we must have an escape char at this point
            buff++;
            switch (*buff)
            {
            case 'b': {byte = '\b'; break;}
            case 'f': {byte = '\f'; break;}
            case 'n': {byte = '\n'; break;}
            case 'r': {byte = '\r'; break;}
            case 't': {byte = '\t'; break;}
            case '\\': {byte = '\\'; break;}
            case '\"': {byte = '\"'; break;}            
            default: { assert (0); break; } // should never happen
            }
        } else { byte = *buff; }
        fprintf(out, " %d,", byte);
        buff++;
    }
    fprintf(out, " 0\n.text\n");
    stringcounter++;

    return label; // returns generated strying label name
}

int branchcounter = 0;
void evalExpression(ASTNode * node, char * truebranchlabel, char * falsebranchlabel) {
    if (node == NULL) return;

    // basecase1: literal
    if (node->node_type == Expr && node->kind.exp == BasicLit) {
        if (node->type_name == INT) {
            int immediate = node->val.ival;
            fprintf(out, "\tli $t0, %d\n", immediate);
        }
        else if (node->type_name == STR) { 
            char * buff = strdup(node->val.sval);
            // remove opening and closing quotation marks
            assert (strlen(buff) >= 2); // scanner lex.l does not remove openign and closing ""
            char * stringonly = buff+1;
            stringonly[strlen(stringonly)-1] = '\0';
            
            char * label = addString(stringonly);
            fprintf(out, "\tla $t0, %s # loading address of 1st char of string `%s`\n", label, stringonly);
            free(buff); // not needed anymore
            free(label); // free as not needed anymore
        } else { assert (0); }
    }

    // base case2: identifier
    if (node->node_type == Expr && node->kind.exp == Id) {
        // check if global var or local
        char * varname = node->val.sval;
        ScopeValue * value = (ScopeValue *) node->sym;
        char * vartype = value->type;

        int size = 1; char * unique_name = (char *) malloc(size); unique_name[0] = '\0';
        size = mystrcat(&unique_name, varname, size);
        size = mystrcat(&unique_name, value->provenience, size);

        int * offset = hashMapFind(table, unique_name);
        // case global: get address of label
        if (offset == NULL) {
            fprintf(out, "\tla $t0, %s # load content of glob var %s %s\n", unique_name, varname, vartype);
            writei("lw $t0, 0($t0) # load address of 1st char");
        }
        // case local: use table hashmap to get offset
        else {
            fprintf(out, "\tlw $t0, %d($fp) # load content of %s %s\n", *offset, varname, vartype);
        }

    }

    // case binary op
    if (node->node_type == Expr && node->kind.exp == BinaryExp) {
        // Remember for || and && there is SHORT CIRCUIT
            // must create branchcounter labels for it
        // Remember for comparing operators
            // int == int: easy
            // bool == bool: easy
            // string == string: not easy
                // but can be made easier
                // in TEMPLATE.s hardcode a subroutine to compare strings
                // the subroutine will return true or false
                // note: strings are compared lexicographically
    }

    // case unary op
    if (node->node_type == Expr && node->kind.exp == UnaryExp) {
        // here it is flip sign

        // evaluate nested expression and then negate it
    }

    // case function call
    if (node->node_type == Expr && node->kind.exp == FuncCall) {
        applyFunctionCall(node);
    }


    // by this point all cases should be covered
    
}