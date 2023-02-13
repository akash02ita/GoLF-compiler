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
    T_BREAK = 1,
    T_ELSE,
    T_FOR,
    T_FUNC,
    T_IF,
    T_RET,
    T_VAR,

    T_ADD,
    T_SUB,
    T_MULT,
    T_DIV,
    T_MOD,

    T_AND,
    T_OR,
    T_EE,
    T_LT,
    T_GT,
    T_EQ,
    T_EMARK,

    T_NE,
    T_LE,
    T_GE,

    T_LP,
    T_RP,
    T_LC,
    T_RC,

    T_C, // comma
    T_S, // semicolon

    T_ID,
    T_INT,
    T_STRING
  };

  Token lastToken = (Token) 0; // default last token value: semicolon should not affect program
  static char const *tokenToString(Token const);
  void myUnput(const char * text);
  void myUnput(const char * text, int si, int ei);
  bool handleImplicitSemicolon(std::string condition, std::string input);
  void handleWarningUnknown(const char * text);
  void handleWarningUnknown(const char * text, int mylen);
  void handleWarningSkip(const char * text);
  void handleWarningSkip(const char * text, int mylen);
};
