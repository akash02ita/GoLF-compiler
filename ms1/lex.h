#pragma once

#include <iostream>
#include <string>
#include "global.h"

#if MODE == SCANNER_MODE
// best to keep it outside class (stays consistent with C) and allows to easily cast G_enum_tok_t in main.cpp
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
#elif MODE == PARSER_MODE
// include dependencies for parser code (bison header generated file)
#include "parse.tab.h" // this let's compiler know where G_enum_tok_t is coming from
#endif

// extern keywoards not allowed inside c++ class. lastToken is defined here to allow any other program to access or print it for DEBUGGING
// although this would be never needed. Reason: lexer will always update it before returning value to yylex(). So lasttoken will always be == current returned token
extern G_enum_tok_t lastToken; // the LEXER must update the lasttoken. since this header file may be imported by multiple files it is best to make it external var to avoid conflicts

namespace GoLF {
  class Lexer : public yyFlexLexer
  {
  public:
    int myLineNo;

    Lexer(std::istream &istream, std::ostream &ostream);

    int yylex();
    
    std::string getAttribute();
    static char const *tokenToString(G_enum_tok_t const);
    void myUnput(const char * text);
    void myUnput(const char * text, int si, int ei);
    bool handleImplicitSemicolon(std::string condition, std::string input = ""); // default arg declared here in header file
    void handleWarningUnknown(const char * text);
    void handleWarningUnknown(const char * text, int mylen);
    void handleWarningSkip(const char * text);
    void handleWarningSkip(const char * text, int mylen);
  };
}
