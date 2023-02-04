#pragma once

#include <iostream>
#include <string>

class MyFlexLexer : public yyFlexLexer
{
public:
  int myLineNo;

  MyFlexLexer(std::istream &istream, std::ostream &ostream);

  int yylex();

  std::string getAttribute();

  enum Token : int
  {
    T_GT = 1,
    T_EQ,
    T_GE,
    T_DOT,
    T_BEGIN,
    T_END,
    T_IDENTIFIER,
    T_STRING,
  };

  static char const *tokenToString(Token const);
};
