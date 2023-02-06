
#include <FlexLexer.h>
#include "lex.h"

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char **argv)
{
  std::istream *input = &std::cin;
  std::ifstream file;
  if (argc == 2)
  {
    file.open(argv[1]);
    if (!file.is_open())
    {
      std::cerr << "failed opening file\n";
      return EXIT_FAILURE;
    }

    input = &file;
  }

  class MyFlexLexer lexer = MyFlexLexer{*input, std::cout};

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

    // update last token
    std::cout << "last token was " << lexer.lastToken << std::endl;
    lexer.lastToken = (MyFlexLexer::Token)tk;
  }
  return EXIT_SUCCESS;
}
