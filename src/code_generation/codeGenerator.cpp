#include <codeGenerator.hpp>

using std::cout, std::string, std::cerr;

using HandlerFunc = std::function<std::string(const std::vector<Token> &)>;

// Dispatch table
std::unordered_map<Tokens, HandlerFunc> handlers = {
    {PRINT, Implementations::print},
    {LET, Implementations::let},
    {SET, Implementations::set},
    {LABEL, Implementations::label},
    {GOTO, Implementations::goTo},
    {BIND, Implementations::bind},
    {IF, Implementations::ifStatement},
    {ADD, Implementations::add},
    {SUB, Implementations::sub},
    {CLEAR, Implementations::clear},
    {PAUSE, Implementations::pause},
    {DY_LABEL, Implementations::dyLabel},
    {USING, Implementations::_using},
    {COLOR, Implementations::color},
    {TITLE, Implementations::title},
    {DELAY, Implementations::delay},
    {INPUT, Implementations::input}};

CompilerContext CodeGenerator::context;

Dy_label *CodeGenerator::findDyLabel(const string &name)
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
    auto it = handlers.find(tokens[0].type);
    if (it != handlers.end())
    {
        return it->second(tokens);
    }

    return "";
}

class PreCompiler
{
public:
    static void DyLabel(const std::vector<Token> &line)
    {
        CodeGenerator::context.dy_labels.push_back(Dy_label(line[1].content));
    }
    static void Bind(const std::vector<Token> &line)
    {
        if (line[2].content == "->")
        {
            Dy_label &dy_label = *CodeGenerator::findDyLabel(line[1].content);
            dy_label.binds.push_back(line[3].content);
        }
    }
    static bool Label(const std::vector<Token> &line)
    {
        if (line[1].type == IDENTIFIER)
        {
            if (std::find(CodeGenerator::context.labels.begin(), CodeGenerator::context.labels.end(), line[1].content) == CodeGenerator::context.labels.end())
            {
                CodeGenerator::context.labels.push_back(line[1].content);
            }
            else
            {
                cerr << "You can´t delcare a label (" + line[1].content + ") twice.\n";
                return false;
            }
        }
        return true;
    }
};

string CodeGenerator::generate(const std::vector<std::vector<Token>> &tokens)
{
    // Default includes and variables
    string program = "#include <stdio.h>\n#include <stdlib.h>\n#include <locale.h>\n#include <string.h>\n#ifdef _WIN32\n#include <windows.h>\n#endif\nint main(){\nchar* __global_color__ = \"\033[0m\";\n";

    for (const auto &line : tokens)
    {
        switch (line[0].type)
        {
        case DY_LABEL:
            PreCompiler::DyLabel(line);
        case BIND:
            PreCompiler::Bind(line);
        case LABEL:
            if (!PreCompiler::Label(line))
                return "int main(){\n// Pre-compiling error\n}";
        default:
            break;
        }
    }

    for (const auto &line : tokens)
        program += generateLine(line);

    program += "return 0;}";

    return program;
}