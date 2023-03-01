#include "parse.tab.h"
#include <stdlib.h>
#include <stdio.h>

int yylex();
int main() {
    yytoken_kind_t token;
    while ((token = yylex()) != 0) {
        fprintf(stdout, "Lexer parsed token %d\n", token);
    }
    // if (yyparse() != 0) {
    //     return EXIT_FAILURE;
    // }
}