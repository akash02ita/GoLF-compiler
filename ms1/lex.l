%option noyywrap
%option c++
%option yyclass="MyFlexLexer"

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
        case T_BREAK:
          return "T_BREAK";
        case T_ELSE:
          return "T_ELSE";
        case T_FOR:
          return "T_FOR";
        case T_FUNC:
          return "T_FUNC";
        case T_IF:
          return "T_IF";
        case T_RET:
          return "T_RET";
        case T_VAR:
          return "T_VAR";
        case T_ADD:
          return "T_ADD";
        case T_SUB:
          return "T_SUB";
        case T_MULT:
          return "T_MULT";
        case T_DIV:
          return "T_DIV";
        case T_MOD:
          return "T_MOD";
        case T_AND:
          return "T_AND";
        case T_OR:
          return "T_OR";
        case T_EE:
          return "T_EE";
        case T_LT:
          return "T_LT";
        case T_GT:
          return "T_GT";
        case T_EQ:
          return "T_EQ";
        case T_EMARK:
          return "T_EMARK";
        case T_NE:
          return "T_NE";
        case T_LE:
          return "T_LE";
        case T_GE:
          return "T_GE";
        case T_LP:
          return "T_LP";
        case T_RP:
          return "T_RP";
        case T_LC:
          return "T_LC";
        case T_RC:
          return "T_RC";
        case T_C:
          return "T_C";
        case T_S:
          return "T_S";
        case T_ID:
          return "T_ID";
        case T_INT:
          return "T_INT";
        case T_STRING:
          return "T_STRING";
        default:
          return "<bad token>";
      }
    }

%}

alph [a-zA-Z]
alphNum [a-zA-Z0-9]

%x STRING

%%
[ \t]+     {}

\"         { BEGIN(STRING); yymore(); }

<STRING>\" { BEGIN(INITIAL); return MyFlexLexer::T_STRING; }
<STRING>\n { fprintf(stderr, "lexer error %s", yytext); BEGIN(INITIAL); }
<STRING>.  { yymore(); }


\n         { ++myLineNo; }

>          { return MyFlexLexer::T_GT; }
=          { return MyFlexLexer::T_EQ; }
>=         { return MyFlexLexer::T_GE; }

{alph}({alphNum}*)  { return MyFlexLexer::T_ID; }

.          { std::cerr << "bad token `" << getAttribute() << "`" << " at " << myLineNo << "." << std::endl; exit(EXIT_FAILURE); }
%%
