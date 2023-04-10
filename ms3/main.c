#include "parse.tab.h"
#include <stdlib.h>
#include <stdio.h>
#include "lex.h"
#include "semantic.h"

// #define ALLOW_STDIN

int yylex();
int main(int argc, char** argv) {
#ifndef ALLOW_STDIN
    if (argc < 2) {
        fprintf(stderr, "Too few arguments. Usage ./golf <filename>\n");
        return EXIT_FAILURE;
    }
    if (argc > 2) {
        fprintf(stderr, "Too many arguments. Usage ./golf <filename>\n");
        return EXIT_FAILURE;
    }
    yyin = fopen(argv[1], "r");
    const char * filename = argv[1];
#else
    yyin = stdin;
    const char * filename = "stdin";
#endif

    if (yyin == NULL) {
        fprintf(stderr, "Could not open file!\n");
        exit(EXIT_FAILURE);
    }
    
    yytoken_kind_t token;
    // while ((token = yylex()) != 0) {
    //     fprintf(stdout, "Lexer scanned token %s", tokenToString(token));
    //     fprintf(stdout, " with attribute %s", getAttribute());
    //     fprintf(stdout, " at or near line %d\n", yylineno);

    //     lastToken = token;
    // }
    if (yyparse() != 0) {
        return EXIT_FAILURE;
    }
    /*
    ASTNode * filenameProgTree = newProg(filename);
    // append(filenameProgTree, progTree);
    filenameProgTree->children[0] = progTree;
    printTree(filenameProgTree, stdout, 1);
    // printTree(progTree, stdout, 1);
    */

    semantic(progTree);
    ASTNode * filenameProgTree = newProg(filename);
    filenameProgTree->children[0] = progTree;
    // printTree(filenameProgTree, stdout, 1);
    
}