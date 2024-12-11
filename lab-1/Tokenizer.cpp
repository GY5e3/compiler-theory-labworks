#include "Calculator.hpp"

std::vector<Token> Tokenizer::operator()(const std::string &data)
{

    std::string operators = "+-*/";

    std::vector<Token> tokens;

    std::string buffer;
    TokenType tokenTypeBuffer;

    State currentState = State::StartState;

    for (int i = 0; i < data.size(); i++)
    {
        char c = data[i];

        if (c == ' ')
            continue;

        bool isDigit = '0' <= c && c <= '9';
        bool isLetter = 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z';
        bool isLParanth = c == '(';
        bool isRParanth = c == ')';
        bool isParanth = isLParanth || isRParanth;
        bool isPoint = c == '.';
        bool isSep = c == ',';
        bool isOperator = operators.find(c) != operators.npos;

        if(!isDigit && !isLetter && !isParanth && !isPoint && !isSep && !isOperator)
            throw std::invalid_argument("Incorrect symbol");

        switch (currentState)
        {
        case State::StartState:
            if (isOperator || isParanth)
                currentState = State::ReadingParenthesisOrOperator;
            else if (isDigit)
                currentState = State::ReadingInteger;
            else if (isLetter)
                currentState = State::ReadingFunction;
            else if (isPoint)
                currentState = State::ReadingFloat;
            else if (isSep)
                throw std::invalid_argument("Uncorrect character: " + std::to_string(i + 1) + "\n");
            break;
        case State::ReadingParenthesisOrOperator:
            if (c == '(' || c == ')')
                currentState = State::ReadingParenthesisOrOperator;
            else if (isDigit)
                currentState = State::ReadingInteger;
            else if (isLetter)
                currentState = State::ReadingFunction;
            else if (isPoint)
                currentState = State::ReadingFloat;
            else if (isSep)
                throw std::invalid_argument("Uncorrect character: " + std::to_string(i + 1) + "\n");
            break;

        case State::ReadingInteger:
            tokenTypeBuffer = TokenType::Integer;
            if (isDigit)
                currentState = State::ReadingInteger;
            else if (isPoint)
                currentState = State::ReadingFloat;
            else if (isOperator || isSep || isParanth)
                currentState = State::WriteInTokensArr;
            else if (isLetter)
                throw std::invalid_argument("Uncorrect character: " + std::to_string(i + 1) + "\n");
            break;

        case State::ReadingFloat:
            tokenTypeBuffer = TokenType::Float;
            if (isDigit)
                currentState = State::ReadingFloat;
            else if (isOperator || isSep || isParanth)
                currentState = State::WriteInTokensArr;
            else if (isPoint)
                throw std::invalid_argument("Uncorrect character: " + std::to_string(i + 1) + "\n");
            break;

        case State::ReadingFunction:
            tokenTypeBuffer = TokenType::Function;
            if ('0' <= c && c <= '9' || 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z')
                currentState = State::ReadingFunction;
            else if (isLParanth)
                currentState = State::WriteInTokensArr;
            else if (isOperator || isRParanth || isSep)
                throw std::invalid_argument("Uncorrect character: " + std::to_string(i + 1) + "\n");
            break;

        case State::WriteInTokensArr:
            if (isParanth || isOperator)
                currentState = State::ReadingParenthesisOrOperator;
            else if (isDigit)
                currentState = State::ReadingInteger;
            else if (isLetter)
                currentState = State::ReadingFunction;
            else if (c == '.')
                currentState = State::ReadingFloat;
            else if (isSep)
                throw std::invalid_argument("Uncorrect character: " + std::to_string(i + 1) + "\n");
            break;

        default:
            throw std::invalid_argument("Unknown character: " + std::to_string(i + 1) + "\n");
            break;
        }

        auto tokenize_Op_Paranth_Sep = [&]()
        {
            if (isOperator)
            {
                // обработка unary negation
                if (tokens.size() == 0 || tokens[tokens.size() - 1].GetTokenType() == TokenType::LeftParanthesis)
                    tokens.push_back({std::string{c}, TokenType::Operator, OperatorAssociativity::Right});
                else
                    tokens.push_back({std::string{c}, TokenType::Operator, OperatorAssociativity::Left});
            }
            else if (isParanth)
            {
                tokens.push_back({std::string{c}, isRParanth ? TokenType::RightParanthesis : TokenType::LeftParanthesis});
            }
            else if (isSep)
            {
                tokens.push_back({std::string{c}, TokenType::Separator});
            }
        };

        switch (currentState)
        {
        case State::ReadingParenthesisOrOperator:
            tokenize_Op_Paranth_Sep();
            break;
        case State::ReadingInteger:
        case State::ReadingFloat:
        case State::ReadingFunction:
            buffer.push_back(c);
            break;
        case State::WriteInTokensArr:
            tokens.push_back(Token(buffer, tokenTypeBuffer));
            buffer.clear();
            // tokenTypeBuffer = specialSymbolType(c);
            // tokens.push_back(Token(std::string(1, c), tokenTypeBuffer));
            tokenize_Op_Paranth_Sep();
            break;
        default:
            throw std::invalid_argument("Unknown state: " + std::to_string(i + 1) + "\n");
        }
    }
    if (buffer != "")
        tokens.push_back(Token(buffer, TokenType::Float));

    return tokens;
}
TokenType Tokenizer::specialSymbolType(const char &symbol)
{
    switch (symbol)
    {
    case '(':
        return TokenType::LeftParanthesis;
    case ')':
        return TokenType::RightParanthesis;
    case ',':
        return TokenType::Separator;
    default:
        return TokenType::Operator;
    }
}