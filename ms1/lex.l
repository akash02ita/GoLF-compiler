%option noyywrap
%option c++
%option yyclass="MyFlexLexer"
%option yylineno
%{
#include"lex.h"
#include<cstdlib>


MyFlexLexer::MyFlexLexer(std::istream& istream, std::ostream& ostream)
    : yyFlexLexer(istream, ostream), myLineNo{1}
{
}

std::string MyFlexLexer::getAttribute() {
  return std::string(YYText(), YYLeng());
}

char const* MyFlexLexer::tokenToString(Token const tok) {
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


void MyFlexLexer::myUnput(const char * text) {
  int i;
  /* Copy yytext because unput() trashes yytext */
  char *yycopy = strdup( text );
  for ( i = yyleng - 1; i >= 0; --i )
      unput( yycopy[i] );
  free( yycopy );
}

void MyFlexLexer::myUnput(const char * text, int si, int ei) {
  if ((ei-si) <= (int) strlen(text)) {
    char* subtext = (char*) malloc((ei-si+1) * sizeof(char));
    strncpy(subtext, text + si, ei - si);
    myUnput(subtext);
    return;
  }
  fprintf(stderr, "myUnput range is longer than input text!");
  exit(EXIT_FAILURE);

}
bool MyFlexLexer::handleImplicitSemicolon(std::string condition, std::string input = "") {
  // std::cout << "\t\tDEBUG: handleimplicit called on condition {" << condition << "}" << std::endl;
  bool flag = false;
  if (condition == "}") flag = MyFlexLexer::lastToken != 0 && MyFlexLexer::lastToken != MyFlexLexer::T_S;
  else { // condition = \n or EOF
    switch(MyFlexLexer::lastToken) {
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
%}

alph [a-zA-Z]
alphNum [a-zA-Z0-9]

non_ascii_char [^\x00-\x7F]
ascii_char .

%x STRING

%%
[ \t]+     {}
\n         { ++myLineNo; if (MyFlexLexer::handleImplicitSemicolon("\n", yytext)) return MyFlexLexer::T_S;}
<INITIAL><<EOF>>    { if (MyFlexLexer::handleImplicitSemicolon("EOF")) return MyFlexLexer::T_S; return 0; }

"+"        { return MyFlexLexer::T_ADD; }
"-"        { return MyFlexLexer::T_SUB; }
"*"        { return MyFlexLexer::T_MULT; }
"/"        { return MyFlexLexer::T_DIV; }
"%"        { return MyFlexLexer::T_MOD; }

"&&"        { return MyFlexLexer::T_AND; }
"||"        { return MyFlexLexer::T_OR; }

"=="        { return MyFlexLexer::T_EE; }
"<"        { return MyFlexLexer::T_LT; }
">"        { return MyFlexLexer::T_GT; }
"="        { return MyFlexLexer::T_EQ; }
"!"        { return MyFlexLexer::T_EMARK; }

"!="        { return MyFlexLexer::T_NE; }
"<="        { return MyFlexLexer::T_LE; }
">="        { return MyFlexLexer::T_GE; }

"("        { return MyFlexLexer::T_LP; }
")"        { return MyFlexLexer::T_RP; }
"{"        { return MyFlexLexer::T_LC; }
"}"        { 
            if (MyFlexLexer::handleImplicitSemicolon("}", yytext)) return MyFlexLexer::T_S;
            return MyFlexLexer::T_RC; 
           }
","        { return MyFlexLexer::T_C; }
";"        { return MyFlexLexer::T_S; }

break      { return MyFlexLexer::T_BREAK; }
else      { return MyFlexLexer::T_ELSE; }
for      { return MyFlexLexer::T_FOR; }
func      { return MyFlexLexer::T_FUNC; }
if      { return MyFlexLexer::T_IF; }
return      { return MyFlexLexer::T_RET; }
var      { return MyFlexLexer::T_VAR; }

"//"{ascii_char}*       {} // skip comments: TODO: how to handles comments with 8-bit char?

{alph}({alphNum}*)  { return MyFlexLexer::T_ID; } // identifier
[0-9][0-9]*         { return MyFlexLexer::T_INT; }   // int literal


\"                                    { BEGIN(STRING); yymore(); }
<STRING>\"                            { BEGIN(INITIAL); return MyFlexLexer::T_STRING; }
<STRING>\n                            { std::cerr << "error: string [" << std::string(yytext).substr(0, yyleng-1) << "] terminated by newline at or near line " << myLineNo << std::endl; exit(EXIT_FAILURE); }
<STRING>\b|\\b|\f|\\f|\\n|\r|\\r|\t|\\t|\\\\|\\\"           { yymore(); }
<STRING>{non_ascii_char}              { std::cerr << "error: string contain non-7-bit ascii_char at or near line " << myLineNo << std::endl; exit(EXIT_FAILURE); }
<STRING><<EOF>>                       {std::cerr << "error: string [" << yytext << "] terminated by EOF at or near line " << myLineNo << std::endl; exit(EXIT_FAILURE);} // TODO: yytext is empty on EOF
<STRING>\\.                           { std::cerr << "error: bad string escape '" << yytext[yyleng-1] << "' at or near line " << myLineNo << std::endl; exit(EXIT_FAILURE); }
<STRING>.                             { yymore(); } // everything else is valid, including _?!''




.          {
    if (++numWarnings < MAX_WARNINGS)  {
      std::cerr << "warning: skipping unknown character `" << getAttribute() << "`" << " at or near line " << myLineNo << std::endl; 
    } else {
      std::cerr << "error: too many warnings at or near line " << myLineNo << std::endl;
      exit(EXIT_FAILURE);
    } 
  }
%%
