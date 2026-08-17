#include <implementations.hpp>
#include <codeGenerator.hpp>
#include <iostream>

using std::cerr, std::string;

std::string Implementations::print(const std::vector<Token> tokens)
{
    if (tokens.size() != 2)
    {
        cerr << tokens[0].content + " is used in this way:\n" + tokens[0].content + " <content>\n";
        return "";
    }
    Tokens type;

    if (tokens[1].type == IDENTIFIER)
    {
        auto it = CodeGenerator::context.variables.find(tokens[1].content);
        if (it == CodeGenerator::context.variables.end())
        {
            cerr << "Error: variable '"
                 << tokens[1].content
                 << "' was not declared.\n";

            return "";
        }
        type = it->second;
    }
    else
        type = tokens[1].type;

    if (type == INT)
        return string("printf(\"%s%d\033[0m") + (tokens[0].content == "println" ? "\\n" : "") + "\",__global_color__, " + tokens[1].content + ");\n";

    if (type == STRING)
        return string("printf(\"%s%s\033[0m") + (tokens[0].content == "println" ? "\\n" : "") + "\",__global_color__, " +
               (tokens[1].type == STRING
                    ? "\"" + tokens[1].content + "\""
                    : tokens[1].content) +
               ");\n";
    return "";
}

std::string Implementations::let(const std::vector<Token> tokens)
{
    if (tokens[1].type == IDENTIFIER && tokens[2].type == EQUALS && (tokens[3].type == STRING || tokens[3].type == INT))
    {
        Tokens type = tokens[3].type;

        if (CodeGenerator::context.variables.find(tokens[1].content) != CodeGenerator::context.variables.end()) // Verify if the variable has been already declared
        {
            cerr << "Can´t declare the same variable (" + tokens[1].content + ") twice.\n";
            return "";
        }

        CodeGenerator::context.variables.emplace(tokens[1].content, type == INT ? INT : STRING);
        return (type == INT ? "int " : "char ") + tokens[1].content + (type == INT ? "int = " : "[1024] = ") + (type == STRING ? "\"" + tokens[3].content + "\"" : tokens[3].content) + ";\n";
    }
    else if (tokens[1].type == IDENTIFIER && (tokens[3].content == "string" || tokens[3].content == "int"))
    {
        Tokens type = (tokens[3].content == "string" ? STRING : INT);

        if (CodeGenerator::context.variables.find(tokens[1].content) != CodeGenerator::context.variables.end()) // Verify if the variable has been already declared
        {
            cerr << "Can´t declare the same variable (" + tokens[1].content + ") twice.\n";
            return "";
        }

        CodeGenerator::context.variables.emplace(tokens[1].content, type == INT ? INT : STRING);
        return (type == INT ? "int " : "char ") + tokens[1].content + (type == INT ? ";\n" : "[1024];\n");
    }
    return "";
}

std::string Implementations::set(const std::vector<Token> tokens)
{
    if (CodeGenerator::context.variables.find(tokens[1].content) == CodeGenerator::context.variables.end()) // Checks if the variable exists
    {
        cerr << "The variable must exist for you to assign something to it.\n";
        return "";
    }

    Tokens variableType = CodeGenerator::context.variables.find(tokens[1].content)->second;
    if (variableType == INT)
        return tokens[1].content + " = " + tokens[3].content + ";\n";
    else if (variableType == STRING)
        return "strcpy(" + tokens[1].content + ",\"" + tokens[3].content + "\");\n";
    return "";
}

std::string Implementations::label(const std::vector<Token> tokens)
{
    if (tokens.size() != 2)
    {
        cerr << "Label is used in this way:\nlabel <label_name>\n";
        return "";
    }
    if (tokens[1].type == IDENTIFIER)
    {
        if (std::find(CodeGenerator::context.labels.begin(), CodeGenerator::context.labels.end(), tokens[1].content) != CodeGenerator::context.labels.end())
        {
            return tokens[1].content + ":\n";
        }
        else
        {
            cerr << "Unknown error\n";
            return "";
        }
    }
    return "";
}

std::string Implementations::goTo(const std::vector<Token> tokens)
{
    if (tokens[1].type == IDENTIFIER)
    {
        Dy_label *dy_label = CodeGenerator::findDyLabel(tokens[1].content);
        if (dy_label != nullptr)
        {
            string ifStatement = "";

            int i = 1;
            for (const auto &bind : dy_label->binds)
            {
                ifStatement += "if (" + dy_label->name + "_assigned == " + std::to_string(i) + ") goto " + bind + ";\n";
                i++;
            }
            return ifStatement;
        }
        else
        {
            if (std::find(CodeGenerator::context.labels.begin(), CodeGenerator::context.labels.end(), tokens[1].content) == CodeGenerator::context.labels.end())
            {
                cerr << "Label " + tokens[1].content + " doesn´t exist.\n";
                return "";
            }
            return "goto " + tokens[1].content + ";\n";
        }
    }
    return "";
}

std::string Implementations::bind(const std::vector<Token> tokens)
{
    Dy_label *dy_label = CodeGenerator::findDyLabel(tokens[1].content);

    auto it = std::find(dy_label->binds.begin(), dy_label->binds.end(), tokens[3].content);
    int pos;

    if (it != dy_label->binds.end())
    {
        pos = std::distance(dy_label->binds.begin(), it);
    }

    if (dy_label != nullptr)
    {
        return dy_label->name + "_assigned = " + std::to_string(pos + 1) + ";\n";
    }
    return "";
}

std::string Implementations::ifStatement(const std::vector<Token> tokens)
{
    if ((tokens[1].type == INT || tokens[1].type == IDENTIFIER) && tokens[2].type == IDENTIFIER && (tokens[3].type == INT || tokens[3].type == IDENTIFIER) && tokens[4].type == GOTO && tokens[5].type == IDENTIFIER)
        return "if (" + tokens[1].content + tokens[2].content + tokens[3].content + ") goto " + tokens[5].content + ";\n";
    return "";
}

std::string Implementations::add(const std::vector<Token> tokens)
{
    if (tokens[3].content == "->" && tokens[4].type == IDENTIFIER)
        return tokens[4].content + " = " + tokens[1].content + " + " + tokens[2].content + ";\n";
    return "";
}

std::string Implementations::sub(const std::vector<Token> tokens)
{
    if (tokens[3].content == "->" && tokens[4].type == IDENTIFIER)
        return tokens[4].content + " = " + tokens[1].content + " - " + tokens[2].content + ";\n";
    return "";
}

std::string Implementations::clear(const std::vector<Token> tokens)
{
    return "system(\"cls\");\n";
}

std::string Implementations::pause(const std::vector<Token> tokens)
{
    return "system(\"pause\");\n";
}

std::string Implementations::dyLabel(const std::vector<Token> tokens)
{
    return "int " + tokens[1].content + "_assigned" + ";\n";
}

std::string Implementations::_using(const std::vector<Token> tokens)
{
    if (tokens[1].content == "utf8")
        return "SetConsoleOutputCP(CP_UTF8);\nSetConsoleCP(CP_UTF8);\n";
    return "";
}

std::string Implementations::color(const std::vector<Token> tokens)
{
    if (tokens[1].content == "black")
        return "__global_color__=\"\033[30m\";\n";
    if (tokens[1].content == "red")
        return "__global_color__=\"\033[31m\";\n";
    if (tokens[1].content == "green")
        return "__global_color__=\"\033[32m\";\n";
    if (tokens[1].content == "yellow")
        return "__global_color__=\"\033[33m\";\n";
    if (tokens[1].content == "blue")
        return "__global_color__=\"\033[34m\";\n";
    if (tokens[1].content == "magenta")
        return "__global_color__=\"\033[35m\";\n";
    if (tokens[1].content == "cyan")
        return "__global_color__=\"\033[36m\";\n";
    if (tokens[1].content == "white")
        return "__global_color__=\"\033[37m\";\n";
    return "";
}

std::string Implementations::title(const std::vector<Token> tokens)
{
    if (tokens[1].type == STRING || tokens[1].type == IDENTIFIER)
        return "char __command__[100];\nsnprintf(__command__, sizeof(__command__), \"title %s\"," + (tokens[1].type == STRING ? ("\"" + tokens[1].content + "\"") : tokens[1].content) + ");\nsystem(__command__);\n";
    return "";
}

std::string Implementations::delay(const std::vector<Token> tokens)
{
    return "Sleep(" + tokens[1].content + ");\n";
}

std::string Implementations::input(const std::vector<Token> tokens)
{
    if (tokens[1].content == "->")
        return "scanf(\"%s\", " + tokens[2].content + ");\n";
    return "";
}