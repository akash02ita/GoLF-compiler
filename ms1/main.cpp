
#include <FlexLexer.h>
#include "lex.h"

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char **argv)
{
  std::ifstream file{argv[1]};

  if (!file.is_open())
  {
    std::cerr << "failed opening file\n";
    return EXIT_FAILURE;
  }

  class MyFlexLexer lexer = MyFlexLexer{file, std::cout};

  int tk;
  while ((tk = lexer.yylex()) != 0)
  {
    std::cout
        << "Token `" << MyFlexLexer::tokenToString(static_cast<MyFlexLexer::Token>(tk))
        << "` with attribute `"
        << lexer.getAttribute()
        << "`"
        << " at line "
        << lexer.myLineNo
        << "\n";
  }
  return EXIT_SUCCESS;
}
