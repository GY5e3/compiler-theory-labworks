#include "Token.hpp"

Token::Token(const std::string &token,
             const TokenType &tokenType,
             const OperatorAssociativity &opAssociativity) : m_token(token),
                                                             m_tokenType(tokenType)
{
    if (tokenType == TokenType::Operator && opAssociativity == OperatorAssociativity::None)
        throw std::logic_error("Associativity required!");

    else if (tokenType != TokenType::Operator && opAssociativity != OperatorAssociativity::None)
        throw std::logic_error("Non-operator token can't have an associativity!");

    m_opAssociativity = opAssociativity;
}

TokenType Token::GetTokenType() const
{
    return m_tokenType;
}
std::string Token::GetToken() const
{
    return m_token;
}
int Token::GetPriority() const
{
    std::unordered_map<std::string, int> mp_left{
        {"+", 1},
        {"-", 1},
        {"*", 2},
        {"/", 2}};

    std::unordered_map<std::string, int> mp_right{
        {"-", 4} 
    };

    switch (m_opAssociativity)
    {
    case OperatorAssociativity::Left:
        if (mp_left.find(m_token) != end(mp_left))
            return mp_left[m_token];
        else
            throw std::invalid_argument("Unknown Operator!");
        break;
    case OperatorAssociativity::Right:
        if (mp_right.find(m_token) != end(mp_right))
            return mp_right[m_token];
        else
            throw std::invalid_argument("Unknown Operator!");
        break;
    case OperatorAssociativity::None:
        throw std::logic_error("Token " + m_token + " is not an operatator, impossible.");
        break;
    }
    return -1;
}
OperatorAssociativity Token::GetAssociativity() const
{
    return m_opAssociativity;
}