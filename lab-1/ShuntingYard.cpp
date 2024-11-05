#include "Calculator.hpp"

std::vector<Token> ShuntingYard::operator()(const std::vector<Token> &tokens)
{
    std::vector<Token> reversePolishNotation;

    std::stack<Token> stack;
    auto fromStackToRPN = [&]()
    {
        reversePolishNotation.push_back(stack.top());
        stack.pop();
    };
    for (const Token &token : tokens)
    {
        switch (token.GetTokenType())
        {
        case TokenType::Integer:
        case TokenType::Float:
            reversePolishNotation.push_back(token);
            break;
        case TokenType::LeftParanthesis:
        case TokenType::Function:
            stack.push(token);
            break;
        case TokenType::Operator:
            if (!stack.empty())
            {
                while(stack.top().GetTokenType() == TokenType::Operator && ((stack.top().GetPriority() > token.GetPriority())
                        || (stack.top().GetPriority() == token.GetPriority() && token.GetAssociativity() == OperatorAssociativity::Left)))
                {
                    fromStackToRPN();
                    if(stack.empty()) 
                        break;
                }
            }
            stack.push(token);
            break;

        case TokenType::RightParanthesis:
            if (stack.empty())
                throw std::invalid_argument("Non-balanced on paranthesis expression!");
            while (stack.top().GetTokenType() != TokenType::LeftParanthesis)
            {
                fromStackToRPN();
                if (stack.empty())
                    throw std::invalid_argument("Non-balanced on paranthesis expression!");
            }
            stack.pop();
            if (!stack.empty() && stack.top().GetTokenType() == TokenType::Function)
                fromStackToRPN();
            break;

        case TokenType::Separator:
            if (stack.empty())
                throw std::invalid_argument("Paranthesis or separator missed!");
            while (stack.top().GetTokenType() != TokenType::LeftParanthesis)
            {
                fromStackToRPN();
                if (stack.empty())
                    throw std::invalid_argument("Paranthesis-unbalanced expression!");
            }
            break;
        }
    }
    while (!stack.empty())
    {
        //std::cout << "Left: " << stack.top().GetToken() << " " << stack.top().GetTokenType()<< std::endl;
        if (stack.top().GetTokenType() == TokenType::LeftParanthesis)
            throw std::invalid_argument("Paranthesis-unbalanced expression!");
        else
            fromStackToRPN();
    }
    return reversePolishNotation;
}