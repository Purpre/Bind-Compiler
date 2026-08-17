#pragma once
#include <lexer.hpp>
#include <string>
#include <unordered_map>

class Implementations
{
public:
    static std::string print(const std::vector<Token> tokens);
    static std::string let(const std::vector<Token> tokens);
    static std::string set(const std::vector<Token> tokens);
    static std::string label(const std::vector<Token> tokens);
    static std::string goTo(const std::vector<Token> tokens);
    static std::string bind(const std::vector<Token> tokens);
    static std::string ifStatement(const std::vector<Token> tokens);
    static std::string add(const std::vector<Token> tokens);
    static std::string sub(const std::vector<Token> tokens);
    static std::string clear(const std::vector<Token> tokens);
    static std::string pause(const std::vector<Token> tokens);
    static std::string dyLabel(const std::vector<Token> tokens);
    static std::string _using(const std::vector<Token> tokens);
    static std::string color(const std::vector<Token> tokens);
    static std::string title(const std::vector<Token> tokens);
    static std::string delay(const std::vector<Token> tokens);
    static std::string input(const std::vector<Token> tokens);
};