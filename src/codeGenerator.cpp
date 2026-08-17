#include <codeGenerator.hpp>

using std::cout, std::string, std::cerr;

CompilerContext CodeGenerator::context;

Dy_label *findDyLabel(const string &name)
{
    auto it = std::find_if(CodeGenerator::context.dy_labels.begin(),
                           CodeGenerator::context.dy_labels.end(),
                           [&](const Dy_label &t)
                           {
                               return t.name == name;
                           });
    if (it == CodeGenerator::context.dy_labels.end())
    {
        return nullptr;
    }
    return &(*it);
}

string generateLine(const std::vector<Token> tokens)
{
    switch (tokens[0].type)
    {
    case PRINT:
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

        break;
    }
    case LET:
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
        break;
    }
    case SET:
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
        break;
    }
    case LABEL:
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
        break;
    }
    case GOTO:
    {
        if (tokens[1].type == IDENTIFIER)
        {
            Dy_label *dy_label = findDyLabel(tokens[1].content);
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
        break;
    }
    case BIND:
    {
        Dy_label *dy_label = findDyLabel(tokens[1].content);

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
        break;
    }
    case IF:
    {
        if ((tokens[1].type == INT || tokens[1].type == IDENTIFIER) && tokens[2].type == IDENTIFIER && (tokens[3].type == INT || tokens[3].type == IDENTIFIER) && tokens[4].type == GOTO && tokens[5].type == IDENTIFIER)
            return "if (" + tokens[1].content + tokens[2].content + tokens[3].content + ") goto " + tokens[5].content + ";\n";
        break;
    }
    case ADD:
    {
        if (tokens[3].content == "->" && tokens[4].type == IDENTIFIER)
            return tokens[4].content + " = " + tokens[1].content + " + " + tokens[2].content + ";\n";
        break;
    }
    case SUB:
    {
        if (tokens[3].content == "->" && tokens[4].type == IDENTIFIER)
            return tokens[4].content + " = " + tokens[1].content + " - " + tokens[2].content + ";\n";
        break;
    }
    case CLEAR:
    {
        return "system(\"cls\");\n";
        break;
    }
    case PAUSE:
    {
        return "system(\"pause\");\n";
        break;
    }
    case DY_LABEL:
    {
        return "int " + tokens[1].content + "_assigned" + ";\n";
        break;
    }
    case USING:
    {
        if (tokens[1].content == "utf8")
            return "SetConsoleOutputCP(CP_UTF8);\nSetConsoleCP(CP_UTF8);\n";
        break;
    }
    case COLOR:
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
        break;
    }
    case TITLE:
    {
        if (tokens[1].type == STRING || tokens[1].type == IDENTIFIER)
            return "char __command__[100];\nsnprintf(__command__, sizeof(__command__), \"title %s\"," + (tokens[1].type == STRING ? ("\"" + tokens[1].content + "\"") : tokens[1].content) + ");\nsystem(__command__);\n";
        break;
    }
    case DELAY:
    {
        return "Sleep(" + tokens[1].content + ");\n";
        break;
    }
    case INPUT:
    {
        if (tokens[1].content == "->")
            return "scanf(\"%s\", " + tokens[2].content + ");\n";
        break;
    }
    default:
        break;
    }
    return "";
}

string CodeGenerator::generate(const std::vector<std::vector<Token>> &tokens)
{
    // Default includes and variables
    string program = "#include <stdio.h>\n#include <stdlib.h>\n#include <locale.h>\n#include <string.h>\n#ifdef _WIN32\n#include <windows.h>\n#endif\nint main(){\nchar* __global_color__ = \"\033[0m\";\n";

    for (const auto &line : tokens)
    {
        if (line[0].type == DY_LABEL) // Pushes all Dy_labels to the context
            CodeGenerator::context.dy_labels.push_back(Dy_label(line[1].content));
            
        if (line[0].type == BIND && line[2].content == "->") // Add binds to respective dy_labels
        {
            Dy_label &dy_label = *findDyLabel(line[1].content);
            dy_label.binds.push_back(line[3].content);
        }
        if (line[0].type == LABEL && line[1].type == IDENTIFIER)
        {
            if (std::find(CodeGenerator::context.labels.begin(), CodeGenerator::context.labels.end(), line[1].content) == CodeGenerator::context.labels.end())
            {
                CodeGenerator::context.labels.push_back(line[1].content);
            }
            else
            {
                cerr << "You can´t delcare a label (" + line[1].content + ") twice.\n";
                return "int main(){\n// Pre-compiling error\n}";
            }
        }
    }

    for (const auto &line : tokens)
        program += generateLine(line);

    program += "return 0;}";

    return program;
}