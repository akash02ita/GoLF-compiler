#include "parse.tab.h"
#include <stdlib.h>
#include <stdio.h>
#include "lex.h"

int yylex();
int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Too few arguments. Usage ./golf <filename>\n");
        return EXIT_FAILURE;
    }
    if (argc > 2) {
        fprintf(stderr, "Too many arguments. Usage ./golf <filename>\n");
        return EXIT_FAILURE;
    }
    yyin = fopen(argv[1], "r");

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
}