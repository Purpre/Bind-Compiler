#pragma once
#include <vector>
#include <iostream>
#include <optional>
#include <cctype>
#include <unordered_map>

enum Tokens
{
    PRINT,
    LET,
    SET,
    EQUALS,
    IDENTIFIER,
    INT,
    STRING,
    GOTO,
    LABEL,
    IF,
    ADD,
    SUB,
    CLEAR,
    PAUSE,
    COMMENT,
    DY_LABEL,
    BIND,
    USING,
    COLOR,
    TITLE,
    DELAY,
    INPUT

};

struct Token
{
    Token(const std::string &c, Tokens t) : content(c), type(t) {}

    std::string content;
    Tokens type;
};

extern std::unordered_map<std::string, Tokens> keywords;

class Lexer
{
public:
    static std::vector<Token> lex(std::string line);
};