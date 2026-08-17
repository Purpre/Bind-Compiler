#include <iostream>
#include <fstream>
#include <lexer.hpp>
#include <codeGenerator.hpp>
// #include <windows.h>

using std::cout, std::string;

int main(int argc, char *argv[])
{
    if (!(argc > 1))
        return 1;

    std::ifstream program(argv[1]);
    if (!program.is_open()) // Verify if the program exists
    {
        cout << "Could not find \"" << argv[1] << "\"\n";
        return 1;
    }

    std::ofstream compiledProgram("program.c"); // Creates the transpiled C code
    if (!compiledProgram.is_open())
        return 1;

    std::vector<std::vector<Token>> tokenLines;
    string line;

    while (std::getline(program, line))
    {
        if (line.empty())
            continue;

        std::vector<Token> currentLine = Lexer::lex(line);

        if (currentLine[0].type == COMMENT)
            continue;

        tokenLines.push_back(currentLine);
    }

    program.close();

    compiledProgram << CodeGenerator::generate(tokenLines);

    compiledProgram.close();

    return 0;
}