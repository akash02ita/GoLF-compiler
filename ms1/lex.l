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
        case T_GT:
          return "T_GT";
        case T_EQ:
          return "T_EQ";
        case T_GE:
          return "T_GE";
        case T_DOT:
          return "T_DOT";
        case T_BEGIN:
          return "T_BEGIN";
        case T_END:
          return "T_END";
        case T_IDENTIFIER:
          return "T_IDENTIFIER";
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
"."        { return MyFlexLexer::T_DOT; }
begin      { return MyFlexLexer::T_BEGIN; }
end        { return MyFlexLexer::T_END; }

{alph}({alphNum}*)  { return MyFlexLexer::T_IDENTIFIER; }

.          { std::cerr << "bad token `" << getAttribute() << "`" << " at " << myLineNo << "." << std::endl; exit(EXIT_FAILURE); }
%%
