#pragma once

#include <iostream>
#include <vector>
#include <unordered_set>
#include <stack>
#include <cmath>

#include "Token.hpp"
enum State
{
    StartState,
    ReadingParenthesisOrOperator,
    ReadingInteger,
    ReadingFloat,
    ReadingFunction,
    WriteInTokensArr
};
class Tokenizer
{
public:
    std::vector<Token> operator()(const std::string &data);
private:
    TokenType specialSymbolType(const char &symbol);
};

class ShuntingYard
{
public:
    std::vector<Token> operator()(const std::vector<Token> &tokens);
};

class EvaluateRPN
{
public:
    double operator()(const std::vector<Token> &tokens);
};

