%option noyywrap
%option c++
%option yyclass="GoLF::Lexer"
%option yylineno
%{
#include<cstdlib>
#include<ctype.h>
#include<string.h>

#include"global.h"
#include"lex.h"
#if MODE == PARSER_MODE
  #include "parse.tab.h"
  #undef  YY_DECL
  #define YY_DECL int GoLF::Lexer::yylex(GoLF::Parser::semantic_type *yylval,\
  GoLF::Parser::location_type *loc)
#endif

/* last token MUST BE UPDATED IN LEXER ONLY. Otherwise if, for instance, this lex file is used by parser, parser won't be updating it */
// lastToken global var is defined here (so no conflicts since defined only in one place)
G_enum_tok_t lastToken = (G_enum_tok_t) 0; // default last token value: semicolon should not affect program
G_enum_tok_t updateLastToken(G_enum_tok_t const tok); // prototypes in lex declarations

%}

alph [a-zA-Z]
alphNum [a-zA-Z0-9]

non_ascii_char [^\x00-\x7F]

%x STRING
%x COMMENT

%%
[ \t]+     {}
\n         { ++myLineNo; if (GoLF::Lexer::handleImplicitSemicolon("\n", yytext)) return updateLastToken(G_enum_tok_t::T_S);}
<INITIAL><<EOF>>    { if (GoLF::Lexer::handleImplicitSemicolon("EOF")) return updateLastToken(G_enum_tok_t::T_S); return 0; }

"+"        { return updateLastToken(G_enum_tok_t::T_ADD); }
"-"        { return updateLastToken(G_enum_tok_t::T_SUB); }
"*"        { return updateLastToken(G_enum_tok_t::T_MULT); }
"/"        { return updateLastToken(G_enum_tok_t::T_DIV); }
"%"        { return updateLastToken(G_enum_tok_t::T_MOD); }

"&&"        { return updateLastToken(G_enum_tok_t::T_AND); }
"||"        { return updateLastToken(G_enum_tok_t::T_OR); }

"=="        { return updateLastToken(G_enum_tok_t::T_EE); }
"<"        { return updateLastToken(G_enum_tok_t::T_LT); }
">"        { return updateLastToken(G_enum_tok_t::T_GT); }
"="        { return updateLastToken(G_enum_tok_t::T_EQ); }
"!"        { return updateLastToken(G_enum_tok_t::T_EMARK); }

"!="        { return updateLastToken(G_enum_tok_t::T_NE); }
"<="        { return updateLastToken(G_enum_tok_t::T_LE); }
">="        { return updateLastToken(G_enum_tok_t::T_GE); }

"("        { return updateLastToken(G_enum_tok_t::T_LP); }
")"        { return updateLastToken(G_enum_tok_t::T_RP); }
"{"        { return updateLastToken(G_enum_tok_t::T_LC); }
"}"        { 
            if (GoLF::Lexer::handleImplicitSemicolon("}", yytext)) return updateLastToken(G_enum_tok_t::T_S);
            return updateLastToken(G_enum_tok_t::T_RC); 
           }
","        { return updateLastToken(G_enum_tok_t::T_C); }
";"        { return updateLastToken(G_enum_tok_t::T_S); }

break      { return updateLastToken(G_enum_tok_t::T_BREAK); }
else      { return updateLastToken(G_enum_tok_t::T_ELSE); }
for      { return updateLastToken(G_enum_tok_t::T_FOR); }
func      { return updateLastToken(G_enum_tok_t::T_FUNC); }
if      { return updateLastToken(G_enum_tok_t::T_IF); }
return      { return updateLastToken(G_enum_tok_t::T_RET); }
var      { return updateLastToken(G_enum_tok_t::T_VAR); }

"//"                                  { BEGIN(COMMENT); }
<COMMENT>{non_ascii_char}             { std::cerr << "error: comment contains non-7bit asci char at or near line " << myLineNo << std::endl; exit(EXIT_FAILURE); }
<COMMENT>\n                           { myLineNo++; BEGIN(INITIAL);}
<COMMENT>.                            { }

({alph}|_)(({alphNum}|_)*)  { return updateLastToken(G_enum_tok_t::T_ID); } // identifier
[0-9][0-9]*         { return updateLastToken(G_enum_tok_t::T_INT); }   // int literal


\"                                    { BEGIN(STRING); yymore(); }
<STRING>\"                            { BEGIN(INITIAL); return updateLastToken(G_enum_tok_t::T_STRING); }
<STRING>\n                            { std::cerr << "error: string [" << std::string(yytext).substr(0, yyleng-1) << "] terminated by newline at or near line " << myLineNo << std::endl; exit(EXIT_FAILURE); }
<STRING>\b|\\b|\f|\\f|\\n|\r|\\r|\t|\\t|\\\\|\\\"           { yymore(); }
<STRING>{non_ascii_char}              { std::cerr << "error: string contain non-7-bit ascii_char at or near line " << myLineNo << std::endl; exit(EXIT_FAILURE); }
<STRING><<EOF>>                       {std::cerr << "error: string [" << yytext << "] terminated by EOF at or near line " << myLineNo << std::endl; exit(EXIT_FAILURE);} // TODO: yytext is empty on EOF
<STRING>\\.                           { std::cerr << "error: bad string escape '" << yytext[yyleng-1] << "' at or near line " << myLineNo << std::endl; exit(EXIT_FAILURE); }
<STRING>.                             { yymore(); } // everything else is valid, including _?!''

"&"          { std::cerr << "error: bitwise AND not supported in GoLF near or at line " << myLineNo << std::endl; exit(EXIT_FAILURE); }
"|"          { std::cerr << "error: bitwise OR not supported in GoLF near or at line " << myLineNo << std::endl; exit(EXIT_FAILURE); }
\0         { handleWarningSkip("NUL"); }
{non_ascii_char} { handleWarningSkip("non-ASCII input"); }
.          { handleWarningUnknown(yytext, yyleng); }
%%
/* here all functions that were prototypes before or above are now defined here */


G_enum_tok_t updateLastToken(G_enum_tok_t const tok) {
  // update token and return same token
  lastToken = tok;
  return tok;
}

// prototypes for functions/methods below are already defined in header file
GoLF::Lexer::Lexer(std::istream& istream, std::ostream& ostream)
    : yyFlexLexer(istream, ostream), myLineNo{1}
{
}

std::string GoLF::Lexer::getAttribute() {
  // something yyleng < strlen(text) when using unput. Thus, this way is better
  return std::string(YYText(), YYLeng());
}

char const* GoLF::Lexer::tokenToString(G_enum_tok_t const tok) {
      switch (tok) {
        case G_enum_tok_t::T_BREAK:     return "T_BREAK";
        case G_enum_tok_t::T_ELSE:      return "T_ELSE";
        case G_enum_tok_t::T_FOR:       return "T_FOR";
        case G_enum_tok_t::T_FUNC:      return "T_FUNC";
        case G_enum_tok_t::T_IF:        return "T_IF";
        case G_enum_tok_t::T_RET:       return "T_RET";
        case G_enum_tok_t::T_VAR:       return "T_VAR";
        case G_enum_tok_t::T_ADD:       return "T_ADD";
        case G_enum_tok_t::T_SUB:       return "T_SUB";
        case G_enum_tok_t::T_MULT:      return "T_MULT";
        case G_enum_tok_t::T_DIV:       return "T_DIV";
        case G_enum_tok_t::T_MOD:       return "T_MOD";
        case G_enum_tok_t::T_AND:       return "T_AND";
        case G_enum_tok_t::T_OR:        return "T_OR";
        case G_enum_tok_t::T_EE:        return "T_EE";
        case G_enum_tok_t::T_LT:        return "T_LT";
        case G_enum_tok_t::T_GT:        return "T_GT";
        case G_enum_tok_t::T_EQ:        return "T_EQ";
        case G_enum_tok_t::T_EMARK:     return "T_EMARK";
        case G_enum_tok_t::T_NE:        return "T_NE";
        case G_enum_tok_t::T_LE:        return "T_LE";
        case G_enum_tok_t::T_GE:        return "T_GE";
        case G_enum_tok_t::T_LP:        return "T_LP";
        case G_enum_tok_t::T_RP:        return "T_RP";
        case G_enum_tok_t::T_LC:        return "T_LC";
        case G_enum_tok_t::T_RC:        return "T_RC";
        case G_enum_tok_t::T_C:         return "T_C";
        case G_enum_tok_t::T_S:         return "T_S";
        case G_enum_tok_t::T_ID:        return "T_ID";
        case G_enum_tok_t::T_INT:       return "T_INT";
        case G_enum_tok_t::T_STRING:    return "T_STRING";
        default:          return "<bad token>";
      }
    }


void GoLF::Lexer::myUnput(const char * text) {
  int i;
  /* Copy yytext because unput() trashes yytext */
  char *yycopy = strdup( text );
  for ( i = yyleng - 1; i >= 0; --i )
      unput( yycopy[i] );
  free( yycopy );
}

void GoLF::Lexer::myUnput(const char * text, int si, int ei) {
  if ((ei-si) <= (int) strlen(text)) {
    char* subtext = (char*) malloc((ei-si+1) * sizeof(char));
    strncpy(subtext, text + si, ei - si);
    myUnput(subtext);
    return;
  }
  fprintf(stderr, "myUnput range is longer than input text!");
  exit(EXIT_FAILURE);

}

// since method is after being prototype declared and called by lex, default argument must be declared on header file (also it is more consistent and clear for documentation)
bool GoLF::Lexer::handleImplicitSemicolon(std::string condition, std::string input) {
  // std::cout << "\t\tDEBUG: handleimplicit called on condition {" << condition << "}" << std::endl;
  bool flag = false;
  if (condition == "}") flag = lastToken != 0 && lastToken != G_enum_tok_t::T_S;
  else { // condition = \n or EOF
    switch(lastToken) {
      case G_enum_tok_t::T_ID: flag = true;
      case G_enum_tok_t::T_INT: flag = true;
      case G_enum_tok_t::T_STRING: flag = true;
      case G_enum_tok_t::T_BREAK: flag = true;
      case G_enum_tok_t::T_RET: flag = true;
      case G_enum_tok_t::T_RP: flag = true;
      case G_enum_tok_t::T_RC: flag = true;
      default: break;
    }
  }

  if (flag && (condition == "\n" || condition == "}")) {
    // std::cout << "\t\tDEBUG: handle implicit unputting {" << input << "} " << std::endl;
    myUnput(input.c_str());
    // myLine-- is not always right, unless myLine -= occurences of \n in input
    myLineNo = yylineno; // bug fix: must restore linenumber.
    
  }

  // std::cout << "\t\tDEBUG: handle implicit returning flag: " << flag << std::endl;
  return flag;
}

const int MAX_WARNINGS = 11;
int numWarnings = 0;

std::string filterText(const char * text, int mylen) {
  std::string r = "";
  for (int i = 0; i < mylen; i++) {
    char c = text[i];
    // idea source: https://stackoverflow.com/questions/14647979/is-it-possible-to-print-non-printing-characters-with-a-c-specifier
    if (isprint(c)) r += c;
    else {
      switch(c) {
        case '\n': r += "\\n"; break;
        case '\v': r += "\\v"; break;
        case '\t': r += "\\t"; break;
        case '\f': r += "\\f"; break;
        case '\r': r += "\\r"; break;
        case '\0': r += "NUL"; break;
        default:   {char * temp; asprintf(&temp, "\\x%02x", c); r += temp; free(temp); break;}
      }
    }
  }

  return r;
}
void GoLF::Lexer::handleWarningUnknown(const char * text, int mylen) {
  if (++numWarnings < MAX_WARNINGS)  {
    std::cerr << "warning: skipping unknown character `" << filterText(text, mylen) << "`" << " at or near line " << myLineNo << std::endl;
  } else {
    std::cerr << "error: too many warnings at or near line " << myLineNo << std::endl;
    exit(EXIT_FAILURE);
  }
}
void GoLF::Lexer::handleWarningUnknown(const char * text) { handleWarningUnknown(text, strlen(text)); }
void GoLF::Lexer::handleWarningSkip(const char * text, int mylen) {
  if (++numWarnings < MAX_WARNINGS)  {
    std::cerr << "warning: skipping " << filterText(text, mylen) << " character" << " at or near line " << myLineNo << std::endl;
  } else {
    std::cerr << "error: too many warnings at or near line " << myLineNo << std::endl;
    exit(EXIT_FAILURE);
  }
}
void GoLF::Lexer::handleWarningSkip(const char * text) { handleWarningSkip(text, strlen(text)); }
