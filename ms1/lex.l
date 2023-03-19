%option noyywrap
%option c++
%option yyclass="GoLF::Lexer"
%option yylineno
%{
#include"lex.h"
#include<cstdlib>
#include<ctype.h>
#include<string.h>
#include"global.h"

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
\n         { ++myLineNo; if (GoLF::Lexer::handleImplicitSemicolon("\n", yytext)) return updateLastToken(T_S);}
<INITIAL><<EOF>>    { if (GoLF::Lexer::handleImplicitSemicolon("EOF")) return updateLastToken(T_S); return 0; }

"+"        { return updateLastToken(T_ADD); }
"-"        { return updateLastToken(T_SUB); }
"*"        { return updateLastToken(T_MULT); }
"/"        { return updateLastToken(T_DIV); }
"%"        { return updateLastToken(T_MOD); }

"&&"        { return updateLastToken(T_AND); }
"||"        { return updateLastToken(T_OR); }

"=="        { return updateLastToken(T_EE); }
"<"        { return updateLastToken(T_LT); }
">"        { return updateLastToken(T_GT); }
"="        { return updateLastToken(T_EQ); }
"!"        { return updateLastToken(T_EMARK); }

"!="        { return updateLastToken(T_NE); }
"<="        { return updateLastToken(T_LE); }
">="        { return updateLastToken(T_GE); }

"("        { return updateLastToken(T_LP); }
")"        { return updateLastToken(T_RP); }
"{"        { return updateLastToken(T_LC); }
"}"        { 
            if (GoLF::Lexer::handleImplicitSemicolon("}", yytext)) return updateLastToken(T_S);
            return updateLastToken(T_RC); 
           }
","        { return updateLastToken(T_C); }
";"        { return updateLastToken(T_S); }

break      { return updateLastToken(T_BREAK); }
else      { return updateLastToken(T_ELSE); }
for      { return updateLastToken(T_FOR); }
func      { return updateLastToken(T_FUNC); }
if      { return updateLastToken(T_IF); }
return      { return updateLastToken(T_RET); }
var      { return updateLastToken(T_VAR); }

"//"                                  { BEGIN(COMMENT); }
<COMMENT>{non_ascii_char}             { std::cerr << "error: comment contains non-7bit asci char at or near line " << myLineNo << std::endl; exit(EXIT_FAILURE); }
<COMMENT>\n                           { myLineNo++; BEGIN(INITIAL);}
<COMMENT>.                            { }

({alph}|_)(({alphNum}|_)*)  { return updateLastToken(T_ID); } // identifier
[0-9][0-9]*         { return updateLastToken(T_INT); }   // int literal


\"                                    { BEGIN(STRING); yymore(); }
<STRING>\"                            { BEGIN(INITIAL); return updateLastToken(T_STRING); }
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
        case T_BREAK:     return "T_BREAK";
        case T_ELSE:      return "T_ELSE";
        case T_FOR:       return "T_FOR";
        case T_FUNC:      return "T_FUNC";
        case T_IF:        return "T_IF";
        case T_RET:       return "T_RET";
        case T_VAR:       return "T_VAR";
        case T_ADD:       return "T_ADD";
        case T_SUB:       return "T_SUB";
        case T_MULT:      return "T_MULT";
        case T_DIV:       return "T_DIV";
        case T_MOD:       return "T_MOD";
        case T_AND:       return "T_AND";
        case T_OR:        return "T_OR";
        case T_EE:        return "T_EE";
        case T_LT:        return "T_LT";
        case T_GT:        return "T_GT";
        case T_EQ:        return "T_EQ";
        case T_EMARK:     return "T_EMARK";
        case T_NE:        return "T_NE";
        case T_LE:        return "T_LE";
        case T_GE:        return "T_GE";
        case T_LP:        return "T_LP";
        case T_RP:        return "T_RP";
        case T_LC:        return "T_LC";
        case T_RC:        return "T_RC";
        case T_C:         return "T_C";
        case T_S:         return "T_S";
        case T_ID:        return "T_ID";
        case T_INT:       return "T_INT";
        case T_STRING:    return "T_STRING";
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
  if (condition == "}") flag = lastToken != 0 && lastToken != T_S;
  else { // condition = \n or EOF
    switch(lastToken) {
      case T_ID: flag = true;
      case T_INT: flag = true;
      case T_STRING: flag = true;
      case T_BREAK: flag = true;
      case T_RET: flag = true;
      case T_RP: flag = true;
      case T_RC: flag = true;
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
