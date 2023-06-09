%option noyywrap
%option yylineno
%{
#include<stdlib.h> // not anymore cstdlib since switched from C++ to C
#include<ctype.h>
#include<string.h>
#include"parse.tab.h"

// #define _GNU_SOURCE // for asprintf but still asprintf does not work on gcc
#include <stdio.h>

#include "lex.h"
yytoken_kind_t lastToken = (yytoken_kind_t) 0;
// last token must be updated inside lexer only, since parser won't be doing that
yytoken_kind_t updateLastToken(yytoken_kind_t const tok);

const int MAX_WARNINGS = 11;
int numWarnings = 0;


%}

alph [a-zA-Z]
alphNum [a-zA-Z0-9]

non_ascii_char [^\x00-\x7F]

%x STRING
%x COMMENT

%%
[ \t]+     {}
\n         {if (handleImplicitSemicolon("\n", yytext)) return updateLastToken(T_S);}
<INITIAL><<EOF>>    { if (handleImplicitSemicolon("EOF", "")) return updateLastToken(T_S); return 0; }

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
            if (handleImplicitSemicolon("}", yytext)) return updateLastToken(T_S);
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
<COMMENT>{non_ascii_char}             { fprintf(stderr, "error: comment contains non-7bit asci char at or near line %d\n", yylineno); exit(EXIT_FAILURE); }
<COMMENT>\n                           { BEGIN(INITIAL); if (handleImplicitSemicolon("\n", yytext)) return updateLastToken(T_S);} // BUF FIX: forgot to handle implicit semicolon here
<COMMENT>.                            { }

({alph}|_)(({alphNum}|_)*)  { return updateLastToken(T_ID); } // identifier
[0-9][0-9]*         { return updateLastToken(T_INT); }   // int literal


\"                                    { BEGIN(STRING); yymore(); }
<STRING>\"                            { BEGIN(INITIAL); return updateLastToken(T_STRING); }
<STRING>\n                            { char * temp = strdup(yytext); temp[yyleng-1] = '\0'; // ignore newline by overwriting null terminator
                                        fprintf(stderr, "error: string [%s] terminated by newline at or near line %d\n", temp, yylineno); free(temp); exit(EXIT_FAILURE); }
<STRING>\b|\\b|\f|\\f|\\n|\r|\\r|\t|\\t|\\\\|\\\"           { yymore(); }
<STRING>{non_ascii_char}              { fprintf(stderr, "error: string contain non-7-bit ascii_char at or near line %d\n", yylineno); exit(EXIT_FAILURE); }
<STRING><<EOF>>                       {fprintf(stderr, "error: string [%s] terminated by EOF at or near line %d\n", yytext, yylineno); exit(EXIT_FAILURE);} // TODO: yytext is empty on EOF
<STRING>\\.                           { fprintf(stderr, "error: bad string escape '%c' at or near line %d\n", yytext[yyleng-1], yylineno); exit(EXIT_FAILURE); }
<STRING>.                             { yymore(); } // everything else is valid, including _?!''

"&"          { fprintf(stderr, "error: bitwise AND not supported in GoLF near or at line %d\n", yylineno); exit(EXIT_FAILURE); }
"|"          { fprintf(stderr, "error: bitwise OR not supported in GoLF near or at line %d\n", yylineno); exit(EXIT_FAILURE); }
\0         { handleWarningSkip("NUL"); }
{non_ascii_char} { handleWarningSkip("non-ASCII input"); }
.          { handleWarningUnknown2(yytext, yyleng); }
%%

yytoken_kind_t updateLastToken(yytoken_kind_t const tok) {

  // update only here to avoid redundancy
  yylloc.first_line = yylineno;
  yylloc.last_line = yylineno;
  yylval.string = strdup(yytext);

  lastToken = tok;
  return tok;
}

/* All functions are coded here, with prototypes already defined. This avoids many lex.yy.c compiling errrors with gcc */
const char * getAttribute() {
    // printf("\n\t\tULTRADEBUG strlen(yytext):%d and yyleng:%d \n", (int) strlen(yytext), (int) yyleng);
    // instaed of returning yytext return first yyleng characters of yytext.
    // BUGFIX: this ensures that when using unput, extra chars are not printed. probably unput modifies yyleng but not yytext properly.
    char * text = malloc(yyleng * sizeof(char));
    strncpy(text, yytext, yyleng);
    text[yyleng] = '\0';
    return text;
}

char const* tokenToString(yytoken_kind_t const tok) {
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

void myUnput(const char * text) {
  int i;
  /* Copy yytext because unput() trashes yytext */
  char *yycopy = strdup( text );
  for ( i = yyleng - 1; i >= 0; --i )
      unput( yycopy[i] );
  free( yycopy );
}

void myUnput2(const char * text, int si, int ei) {
  if ((ei-si) <= (int) strlen(text)) {
    char* subtext = (char*) malloc((ei-si+1) * sizeof(char));
    strncpy(subtext, text + si, ei - si);
    myUnput(subtext);
    return;
  }
  fprintf(stderr, "myUnput range is longer than input text!");
  exit(EXIT_FAILURE);

}



bool handleImplicitSemicolon(const char * condition, const char * input) {
  const char * STR_EMPTY = "";
  if (input == NULL) {
    // fprintf(stdout, "\t\tDEBUG: handle INPUT IS empty\n");
    input = STR_EMPTY;
  }
  // fprintf(stdout, "\t\tDEBUG: handleimplicit called on condition {%s}\n", condition);
  bool flag = false;
  if (strcmp(condition, "}") == 0) flag = lastToken != 0 && lastToken != T_S;
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

  if (flag && (strcmp(condition,"\n") == 0 || strcmp(condition,"}") == 0)) {
    // fprintf(stdout, "\t\tDEBUG: handle implicit unputting {%s}\n", input);
    myUnput(input);
    
  }

  // fprintf(stdout, "\t\tDEBUG: handle implicing returning flag: %d\n", flag);
  return flag;
}

int mystrcat(char * dest, const char * src, const unsigned int DEST_SIZE) {
  unsigned int NEW_DEST_SIZE = DEST_SIZE;
  if (DEST_SIZE < strlen(dest) + strlen(src)) {
    char * backup = strdup(dest);
    NEW_DEST_SIZE = strlen(dest) + strlen(src);
    dest = (char *) realloc(dest, DEST_SIZE * sizeof(char));
    strcpy(dest, backup);
    free(backup);
  }
  strcat(dest, src);
  return NEW_DEST_SIZE;
}

const char * filterText(const char * text, int mylen) {
  char * r = (char *) malloc(MY_MAX_BUFFER_SIZE * sizeof(char));
  r[0] = '\0';
  unsigned int BUF_SIZE = MY_MAX_BUFFER_SIZE;
  for (int i = 0; i < mylen; i++) {
    char c = text[i];
    // idea source: https://stackoverflow.com/questions/14647979/is-it-possible-to-print-non-printing-characters-with-a-c-specifier
    if (isprint(c)) {
      char * temp = (char *) malloc(2*sizeof(char)); temp[0] = c; temp[1] = '\0';
      mystrcat(r, temp, BUF_SIZE);
      free(temp);
    } else {
      switch(c) {
        case '\n': BUF_SIZE = mystrcat(r, "\\n", BUF_SIZE); break;
        case '\v': BUF_SIZE = mystrcat(r, "\\v", BUF_SIZE); break;
        case '\t': BUF_SIZE = mystrcat(r, "\\t", BUF_SIZE); break;
        case '\f': BUF_SIZE = mystrcat(r, "\\f", BUF_SIZE); break;
        case '\r': BUF_SIZE = mystrcat(r, "\\r", BUF_SIZE); break;
        case '\0': BUF_SIZE = mystrcat(r, "NUL", BUF_SIZE); break;
        // default:   {char * temp; asprintf(&temp, "\\x%02x", c); mystrcat(r, temp, BUF_SIZE); free(temp); break;} // does not work
        default:   {char temp[10]; sprintf(temp, "\\x%02x", c); mystrcat(r, temp, BUF_SIZE); ; break;} // assume formatted string output is smaller than allocated memory
      }
    }
  }

  return r;
}

void handleWarningUnknown2(const char * text, int mylen) {
  if (++numWarnings < MAX_WARNINGS)  {
    fprintf(stderr, "warning: skipping unknown character `%s` at or near line %d\n", filterText(text, mylen), yylineno);
  } else {
    fprintf(stderr, "error: too many warnings at or near line %d\n", yylineno);
    exit(EXIT_FAILURE);
  }
}
void handleWarningUnknown(const char * text) { handleWarningUnknown2(text, strlen(text)); }

void handleWarningSkip2(const char * text, int mylen) {
  if (++numWarnings < MAX_WARNINGS)  {
    fprintf(stderr, "warning: skipping %s character at or near line %d\n", filterText(text, mylen),yylineno);
  } else {
    fprintf(stderr, "error: too many warnings at or near line %d\n", yylineno);
    exit(EXIT_FAILURE);
  }
}
void handleWarningSkip(const char * text) { handleWarningSkip2(text, strlen(text)); }