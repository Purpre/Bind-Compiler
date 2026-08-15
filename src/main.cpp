#include <iostream>
#include <fstream>
#include <lexer.hpp>
#include <codeGenerator.hpp>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (!(argc > 1))
        return 1;

    std::ifstream program(argv[1]);
    if (!program.is_open())
    {
        std::cout << "Could not find \"" << argv[1] << "\"\n";
        return 1;
    }

    std::string line;

    std::ofstream compiledProgram("program.c");
    if (!compiledProgram.is_open())
        return 1;

    std::vector<std::vector<Token>> tokenLines;

    while (std::getline(program, line))
    {
        if (line.empty())
            continue;

        std::vector<Token> currentLine = Lexer::lex(line);

        if (currentLine[0].type == COMMENT)
            continue;

        tokenLines.push_back(Lexer::lex(line));
    }

    program.close();

    compiledProgram << CodeGenerator::generate(tokenLines);

    compiledProgram.close();

    return 0;
}