#include "parse.tab.h"
#include <stdlib.h>
#include <stdio.h>
#include "lex.h"

int yylex();
int main() {
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