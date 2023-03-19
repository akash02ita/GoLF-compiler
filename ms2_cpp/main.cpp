/********************************
 * Example C++ Parser
 * Written for CPSC 411 Tutorial
 * Shankar Ganesh
 * *****************************/


#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <fstream>

#include "lex.h"
#include "parse.tab.h"

int main(int argc, char **argv) 
{

    std::istream *input = &std::cin;

    std::ifstream file;

    if (argc == 2)
    {
        file.open(argv[1]);

        if (!file.good())
        {
            std::cerr << "Error: " << strerror(errno) << "\n";
            return EXIT_FAILURE;
        }

        input = &file;
    }

    auto lexer = std::make_unique<GoLF::Lexer>(*input, std::cout);
    auto parser = std::make_unique<GoLF::Parser>(lexer);

    if(parser->parse() != 0)
    {
        std::cerr << "Parse failed!!\n";
        if (file.is_open()) file.close();
        return EXIT_FAILURE;
    }

    if (file.is_open()) file.close();

    return 0;
}
