%option noyywrap
%{
    /* THIS ONE IS WITH A FUNCTION INSTEAD */
    #include <stdio.h>
    #define true 1
    #define false 0
    #define bool int
    #define STOP 0

    bool is_intlit(const char * txt);
%}

int_lit {decimal_lit}|{binary_lit}
decimal_lit 0|[1-9](_?[0-9])*
binary_lit 0(b|B)(_?[0-1])+

%%

{int_lit}           {   // remove ^ and $ as that prevent matching when using yy_scan_string
                        // fprintf(stderr, "Integer literal matched: %s\n", yytext);
                        return true;
                      }

.*                   { // this part will handle if any other characters are present
                        // fprintf(stderr, "Invalid input %s\n",yytext);
                        return false;
                     }

<INITIAL><<EOF>>     {
                        // fprintf(stderr,"EOF reached\n");
                        return STOP;
                     }
%%

bool is_intlit(const char * txt) {
    yy_scan_string(txt);
    bool t = yylex();
    return t;
}


int main() {
    // use buffer instead of stdin here this time
    char * arr[] = {
        "1_",
        "0123",
        "1234",
        "0",
        "1_2_3",
        "0b",
        "0B",
        "0b_",
        "0B_",
        "0b_1",
        "0B_1",
        "0B_111_1",
        "0b_1234_2",
        " 1234",
        "1234",
        "\t0b110"
    };

    for (int i = 0; i < 16; i++) {
        fprintf(stdout, "%d\t%s\n", is_intlit(arr[i]), arr[i]);
    }
    
    return 0;
}