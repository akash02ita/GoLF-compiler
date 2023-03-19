#define SCANNER_MODE 0
#define PARSER_MODE 1

// enable EXACTLY one of these modes
// #define MODE SCANNER_MODE // using program as SCANNER ONLY
#define MODE PARSER_MODE // using program as PARSER ONLY

#if MODE == SCANNER_MODE
#define G_enum_tok_t Token // custom `global enum token type` for scanner
#elif MODE == PARSER_MODE
#define G_enum_tok_t GoLF::Parser::token::yytokentype // bison -d generated header file has this enum token type
#endif

