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
            // preTraversalPostBeforeNext(node->children[1], buildSigTable, NULL, NULL);
            // preTraversalPostBeforeNext(node->children[1], buildSigTable, NULL, NULL); // problem: it build counter in opposite order
            postTraversal(node->children[1], buildSigTable, NULL); //BUGFIX: the last parameters are closer to $fp!!!
            preTraversalPostBeforeNext(node->children[2], buildLVTable, NULL, NULL);
            allocate(node);

            char * retlabel = strdup(label);
            buffsize = mystrcat(&retlabel, "_ret", buffsize);
            // printf("return label is %s:\n", retlabel);

            // step3: write code for function block
            applyBlock(node->children[2], label, retlabel, NULL);

            // AT THIS POINT, no return statement was called
            // if function is non-void-->trigger dynamic semantic error of no return runtime
            assert(value->rettype != NULL);
            if (strcmp(value->rettype, "bool") == 0 ||
                strcmp(value->rettype, "int") == 0 ||
                strcmp(value->rettype, "string") == 0
            ) {
                char * strlabel = addString(functionname);
                fprintf(out, "\tla $a0, $noreturn1\nli $v0, 4\nsyscall\n");
                fprintf(out, "\tla $a0, %s\nli $v0, 4\nsyscall\n", strlabel);
                fprintf(out, "\tla $a0, $noreturn2\nli $v0, 4\nsyscall\n");
            }

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
        assert (offset != NULL);
        // printf("lvcounter %d? offset %d?  for %s %s\n", lvcounter, *offset, varname, unique_name);
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
    if (blocknode->node_type == Decl && blocknode->kind.decl == VarDecl) {
        // local vardeclarations are NOT skipped: needs to be reset to default value
        char * varname = blocknode->children[0]->val.sval;
        ScopeValue * value = (ScopeValue *) blocknode->sym;
        int size = 1; char * unique_name = (char *) malloc(size); unique_name[0] = '\0';
        size = mystrcat(&unique_name, varname, size);
        size = mystrcat(&unique_name, value->provenience, size);
        int * offset = (int *) hashMapFind(table, unique_name); // NEEDED
        assert (offset != NULL);

        char * type_var = blocknode->children[1]->val.sval;
        // printf("Variable %s is of type %s\n", unique_name, type_var); getchar();
        fprintf(out, "\t# Reset value %s\n", unique_name);
        if (strcmp(type_var, "int") == 0) {
            fprintf(out, "\tsw $zero, %d($fp)\n", *offset);
        }
        else if (strcmp(type_var, "bool") == 0) {
            fprintf(out, "\tsw $zero, %d($fp)\n", *offset);
        }
        else if (strcmp(type_var, "string") == 0) {
            fprintf(out, "\tla $t0, $string_empty\n");
            fprintf(out, "\tsw $t0, %d($fp)\n", *offset); // variable will store address of label always!
        }
    }
    else if (blocknode->node_type == Stmt) {
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
            char * skipelse = generateLabel("skipelse", ifcounter);

            ifcounter++;

            // write code for condition checking
            evalExpression(blocknode->children[0], bodylabel, elselabel);
            // write code for body of if
            fprintf(out, "%s:\n", bodylabel);
            applyBlock(blocknode->children[1], NULL, retlabel, breaklabel);
            // BUGFIX: if else forgot to jump outside else
            fprintf(out, "\tj %s # skip else part\n", skipelse);
            // write code for else of if
            fprintf(out, "%s:\n", elselabel);
            applyBlock(blocknode->children[2], NULL, retlabel, breaklabel);
            fprintf(out, "%s:\n", skipelse);
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
    // NOTE: do not forget to copy paste $t0
    writei("move $t0, $v0");

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
#define INT_TYPE 10
#define STR_TYPE 20
#define BOOL_TYPE 30
int evalExpression(ASTNode * node, char * truebranchlabel, char * falsebranchlabel) {
    /*
        NOTE: whereeveer it returns boolean, the true and false branching must be done if it has to be

        For now recursive calls, have true and branch labels set to null as they are not needed to keep logic correct
            this is a safety measure to prevent any possible weird bugs
        
        When unaryop is ! the recursive call must be with true and false branch labels swapped!
            or pass NULL, NULL to be safe
            this is becuase ! flips bool sign which implies opposite order
    */
    if (node == NULL) return -1;

    // basecase1: literal
    if (node->node_type == Expr && node->kind.exp == BasicLit) {
        if (node->type_name == INT) {
            int immediate = node->val.ival;
            fprintf(out, "\tli $t0, %d # base case int literal\n", immediate);
            return INT_TYPE;
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
            return STR_TYPE;
        } else { assert (0); }
    }

    // base case2: identifier
    if (node->node_type == Expr && node->kind.exp == Id) {
        // check if global var or local
        char * varname = node->val.sval;
        ScopeValue * value = (ScopeValue *) node->sym;
        assert (value != NULL);
        if (value->isid) {
            char * vartype = value->type;
            assert (vartype != NULL);

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
            if (strcmp(vartype, "int") == 0) return INT_TYPE;
            else if (strcmp(vartype, "string") == 0) return STR_TYPE;
            else if (strcmp(vartype, "bool") == 0) {
                if (truebranchlabel != NULL) {
                    // true means != 0
                    fprintf(out, "\tbne $t0, $zero, %s\n", truebranchlabel);
                }
                if (falsebranchlabel != NULL) {
                    // false mean == 0
                    fprintf(out, "\tbeq $t0, $zero, %s\n", falsebranchlabel);
                }
                return BOOL_TYPE;
            }
            else assert (0);
        }
        else if (value->isconst) {
            // true or false
            if (strcmp(varname, "true") == 0) {
                writei("li $t0, 1   # constant write true");
            }
            else if (strcmp(varname, "false") == 0) {
                writei("move $t0, $zero   # constant write false");

            } else assert (0);
            if (truebranchlabel != NULL) {
                // true means != 0
                fprintf(out, "\tbne $t0, $zero, %s\n", truebranchlabel);
            }
            if (falsebranchlabel != NULL) {
                // false mean == 0
                fprintf(out, "\tbeq $t0, $zero, %s\n", falsebranchlabel);
            }
            return BOOL_TYPE;
        } else assert (0);
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
        ASTNode * left = node->children[0];
        ASTNode * right = node->children[1];

        // int typeleft = evalExpression(left, truebranchlabel, falsebranchlabel);
        int typeleft = evalExpression(left, NULL, NULL);
        writei("addi $sp, $sp, -4"); writei("sw $t0, 0($sp) # append lhs result"); // append word on stack
        switch (node->val.op)
        {
            case ADD:
            case SUB:
            case MULT:
            case DIV:
            case MOD: {
                // int typeright = evalExpression(right, truebranchlabel, falsebranchlabel);
                int typeright = evalExpression(right, NULL, NULL);
                writei("addi $sp, $sp, -4"); writei("sw $t0, 0($sp) # append rhs result"); // append word on stack
                // printf("%d %d\n", typeleft, typeright);
                assert (typeleft == typeright && typeleft == INT_TYPE);
                applyIntOp(node->val.op);
                return INT_TYPE;
                break;
            }
            case LT:
            case GT:
            case LTE:
            case GTE:
            case EQEQ:
            case NEQ: {
                // int typeright = evalExpression(right, truebranchlabel, falsebranchlabel);
                int typeright = evalExpression(right, NULL, NULL);
                writei("addi $sp, $sp, -4"); writei("sw $t0, 0($sp) # append rhs result"); // append word on stack
                assert (typeleft == typeright);
                applyRelOp(node->val.op, typeleft);
                if (truebranchlabel != NULL) {
                    // true means != 0
                    fprintf(out, "\tbne $t0, $zero, %s\n", truebranchlabel);
                }
                if (falsebranchlabel != NULL) {
                    // false mean == 0
                    fprintf(out, "\tbeq $t0, $zero, %s\n", falsebranchlabel);
                }
                return BOOL_TYPE;
                break;
            }
            case AND: {
                // short circuit:
                char * skiplabel = generateLabel("skipand_false", branchcounter);
                branchcounter++;
                fprintf(out, "\tbeq $t0, $zero, %s\n", skiplabel); // if false then jump
                // evalExpression(right, truebranchlabel, falsebranchlabel);
                evalExpression(right, NULL, NULL);
                writei("lw $t1, 0($sp)");
                writei("and $t0, $t1, $t0"); // $t0 is result of right, $t1 is saved result of left
                fprintf(out, "%s:\n", skiplabel);
                writei("add $sp, $sp, 4  # remove lhs"); // remove left result on top of stack
                free(skiplabel);
                if (truebranchlabel != NULL) {
                    // true means != 0
                    fprintf(out, "\tbne $t0, $zero, %s\n", truebranchlabel);
                }
                if (falsebranchlabel != NULL) {
                    // false mean == 0
                    fprintf(out, "\tbeq $t0, $zero, %s\n", falsebranchlabel);
                }
                return BOOL_TYPE;
                break;
            }
            case OR: {
                // short circuit:
                char * skiplabel = generateLabel("skipor_true", branchcounter);
                branchcounter++;
                fprintf(out, "\tbne $t0, $zero, %s\n", skiplabel); // if true then jump
                // evalExpression(right, truebranchlabel, falsebranchlabel);
                evalExpression(right, NULL, NULL);
                writei("lw $t1, 0($sp)");
                writei("or $t0, $t1, $t0"); // $t0 is result of right, $t1 is saved result of left
                fprintf(out, "%s:\n", skiplabel);
                writei("add $sp, $sp, 4 # remove lhs"); // remove left result on top of stack
                free(skiplabel);
                if (truebranchlabel != NULL) {
                    // true means != 0
                    fprintf(out, "\tbne $t0, $zero, %s\n", truebranchlabel);
                }
                if (falsebranchlabel != NULL) {
                    // false mean == 0
                    fprintf(out, "\tbeq $t0, $zero, %s\n", falsebranchlabel);
                }
                return BOOL_TYPE;
                break;
            }
            default: { assert(0); break; }
        }
    }

    // case unary op
    if (node->node_type == Expr && node->kind.exp == UnaryExp) {
        // evaluate nested expression and then flip it
        // when unary op is !, then it flips boolean,
            // thus true and false branchlabels should be swapped!
        // evalExpression(node->children[0], truebranchlabel, falsebranchlabel);
        // or it can also work without allowing branching, so that it is handled only at this point
        evalExpression(node->children[0], NULL, NULL);

        // here it is flip int sign if UNARY SIGN is '-'
        if (node->val.op == SUB) {

            // flip sign by doing -$t0 = 0 - $t0
            // writei("sub $t0, $zero, $t0");
            // writei("subu $t0, $zero, $t0"); // works
            writei("negu $t0, $t0");

            return INT_TYPE;
        }
        // here it is to flip bool sign if UNARY sign IS '!'
        else if (node->val.op == EMARK) {
            // xor can be usefule to toogle 0 to 1 or vice versa
            writei("li $t1, 1"); // $t0 xor 1 will toogle bit
            writei("xor $t0, $t0, $t1");
            if (truebranchlabel != NULL) {
                // true means != 0
                fprintf(out, "\tbne $t0, $zero, %s  # unary TRUE\n", truebranchlabel);
            }
            if (falsebranchlabel != NULL) {
                // false mean == 0
                fprintf(out, "\tbeq $t0, $zero, %s  # unary FALSE\n", falsebranchlabel);
            }
            return BOOL_TYPE;
        } else assert(0);
        assert (0); 
    }

    // case function call
    if (node->node_type == Expr && node->kind.exp == FuncCall) {
        applyFunctionCall(node);
        ScopeValue * value = (ScopeValue *) node->sym;
        assert (value->isfunc);
        char * type = value->rettype;
        assert (type != NULL);
        // printf("what is ret type then? %s\n", type);
        if (strcmp(type, "int") == 0) return INT_TYPE;
        else if (strcmp(type, "bool") == 0) return BOOL_TYPE;
        else if (strcmp(type, "string") == 0) return STR_TYPE;
        else assert(0);
    }


    // by this point all cases should be covered
    assert (0);
    printTree(node, stderr, 4);
    getchar();
    return -1;
}


void applyIntOp(Oper op) {
    // pop the 2 words
    writei("# pop 2 words apply op");
    writei("lw $t1, 0($sp)"); // bufix: 2nd operand is t1: so very top of stack!
    writei("addi $sp, $sp, 4");
    writei("lw $t0, 0($sp)"); // bugfix: 1st operand is below!
    writei("addi $sp, $sp, 4");
    switch (op)
    {
    case ADD: {
        // writei("add $t0, $t0, $t1");
        writei("addu $t0, $t0, $t1");
        break;
    }
    case SUB: {
        // writei("sub $t0, $t0, $t1");
        writei("subu $t0, $t0, $t1");
        break;
    }
    case MULT: {
        writei("mult $t0, $t1");
        writei("mflo $t0"); // mflo has lower 32 bits value of multiplication
        break;
    }
    case DIV: {
        // writei("div $t0, $t1");
        writei("beq $t1, $zero, $div_err");
        writei("divu $t0, $t1");
        writei("mflo $t0"); // mflo has quotient
        break;
    }
    case MOD: {
        // writei("div $t0, $t1");
        // writei("divu $t0, $t1");
        // writei("mfhi $t0"); // mflow has remainder
        writei("rem $t0, $t0, $t1");
        break;
    }
    default: {assert (0); break; }
    }
}

void applyRelOp(Oper op, int type) {
    if (type == INT_TYPE) {
        switch (op)
        {   // TEMPLATE.S subourtines
            case LT: { writei("jal intlt"); break;}
            case GT:  { writei("jal intgt"); break;}
            case LTE: { writei("jal intlte"); break;}
            case GTE: { writei("jal intgte"); break;}
            case EQEQ: { writei("jal inteqeq"); break;}
            case NEQ: { writei("jal intneq"); break;}
            default: assert (0); break;
        }
        writei("move $t0, $v0  #  int relop result");
    }
    else if (type == BOOL_TYPE) {
        switch (op)
        {   // TEMPLATE.S subourtines
            case EQEQ: { writei("jal inteqeq"); break;}
            case NEQ: { writei("jal intneq"); break;}
            default: assert (0); break;
        }
        writei("move $t0, $v0    #  bool relop result");
    }
    else if (type == STR_TYPE) {
        switch (op)
        {   // TEMPLATE.S subourtines
            case LT: { writei("jal strlt"); break;}
            case GT:  { writei("jal strgt"); break;}
            case LTE: { writei("jal strlte"); break;}
            case GTE: { writei("jal strgte"); break;}
            case EQEQ: { writei("jal streqeq"); break;}
            case NEQ: { writei("jal strneq"); break;}
            default: {
                assert(0);
                break;
            }
        }
        writei("move $t0, $v0   # string relop result");
    } else assert (0);
    // remove 2 words of stack (args passed)
    writei("# pop 2 words rel op");
    writei("addi $sp, $sp, 4");
    writei("addi $sp, $sp, 4");
}