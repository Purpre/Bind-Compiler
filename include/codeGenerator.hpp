#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <lexer.hpp>

struct Dy_label
{
    inline Dy_label(std::string name) : name(name) {};

    std::string name;
    std::vector<std::string> binds;
};

struct CompilerContext
{
    std::vector<Dy_label> dy_labels;
};

class CodeGenerator
{
public:
    static CompilerContext context;
    static std::string generate(const std::vector<std::vector<Token>> &tokens);
};