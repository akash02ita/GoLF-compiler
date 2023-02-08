
#include <FlexLexer.h>
#include "lex.h"

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char **argv)
{
  if (argc != 2) return 0; // if this line is uncommented, then stdin is not allowed
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
    std::cout << "\t\tlast token was " << MyFlexLexer::tokenToString(lexer.lastToken) << std::endl;
    lexer.lastToken = (MyFlexLexer::Token)tk;
  }
  return EXIT_SUCCESS;
}
