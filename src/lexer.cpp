#include <lexer.hpp>

std::unordered_map<std::string, Tokens> Lexer::variables;

std::unordered_map<std::string, Tokens> keywords = {
    {"print", PRINT},
    {"println", PRINT},
    {"=", EQUALS},
    {"let", LET},
    {"set", SET},
    {"goto", GOTO},
    {"label", LABEL},
    {"if", IF},
    {"add", ADD},
    {"sub", SUB},
    {"clear", CLEAR},
    {"pause", PAUSE},
    {"//", COMMENT},
    {"dy_label", DY_LABEL},
    {"bind", BIND},
    {"using", USING},
    {"color", COLOR},
    {"title", TITLE},
    {"delay", DELAY},
    {"input", INPUT}};

std::vector<std::string> splitString(std::string *text)
{
    std::vector<std::string> strings;
    std::string buffer;

    for (char c : *text)
    {
        if (c == ' ')
        {
            if (!buffer.empty())
                strings.push_back(buffer);

            buffer.clear();
            continue;
        }

        buffer += c;
    }

    if (!buffer.empty())
        strings.push_back(buffer);

    return strings;
}

std::vector<std::string> concatStrings(const std::vector<std::string> &rawSplit)
{
    std::vector<std::string> result;

    bool waitingForQuote = false;
    std::string buffer;

    for (const auto &text : rawSplit)
    {
        if (text.empty())
            continue;

        if (!waitingForQuote && text[0] == '"')
        {
            waitingForQuote = true;
            buffer = text;
        }
        else if (waitingForQuote)
            buffer += " " + text;
        else
            result.push_back(text);
        if (waitingForQuote && text.back() == '"')
        {
            result.push_back(buffer);
            buffer.clear();
            waitingForQuote = false;
        }
    }

    return result;
}

Token classifyToken(std::string text)
{
    if (text.empty())
    {
        std::cout << "TOKEN VAZIO!\n";
        return Token("", IDENTIFIER);
    }
    if (text.at(0) == '"' && text.at(text.size() - 1) == '"')
    {
        text.erase(text.begin());
        text.pop_back();
        return Token(text, STRING);
    }
    else if (keywords.find(text) != keywords.end())
        return Token(text, keywords.find(text)->second);
    else if (std::isdigit(text.at(0)))
        return Token(text, INT);
    else
        return Token(text, IDENTIFIER);
}

std::vector<Token> Lexer::lex(std::string line)
{
    std::vector<Token> instructions;

    auto parts = concatStrings(splitString(&line));

    if (!parts.empty() && parts[0] == "//")
    {
        instructions.push_back(Token("//", COMMENT));
        return instructions;
    }

    for (auto &text : parts)
        instructions.push_back(classifyToken(text));

    return instructions;
}