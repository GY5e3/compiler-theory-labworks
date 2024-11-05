#pragma once

#include <string>
#include <unordered_map>
#include <iostream>

enum TokenType
{
    Operator,
    LeftParanthesis,
    RightParanthesis,
    Integer,
    Float,
    Function,
    Separator
};
enum OperatorAssociativity
{
    None,  
    Right, 
    Left
};
class Token
{
public:
    Token(const std::string &m_token, const TokenType &tokenType, const OperatorAssociativity &opAssociativity = OperatorAssociativity::None);
    int GetPriority() const;
    TokenType GetTokenType() const;
    std::string GetToken() const;
    OperatorAssociativity GetAssociativity() const;
private:
    TokenType m_tokenType;
    OperatorAssociativity m_opAssociativity;
    std::string m_token;
};