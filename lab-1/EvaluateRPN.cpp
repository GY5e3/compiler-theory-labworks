#include "Calculator.hpp"

double EvaluateRPN::operator()(const std::vector<Token> &tokens)
{
    std::stack<double> stack;
    auto getOneToken = [&]()
    {
        if (stack.empty())
            throw std::invalid_argument("Not enough arguments in function!");
        double x = stack.top();
        stack.pop();
        return x;
    };
    auto getTwoTokens = [&]()
    {
        double x = getOneToken(), y = getOneToken();
        return std::pair{y, x};
    };
    auto checkedDivision = [&](double a, double b)
    { if(b == 0.f) throw std::invalid_argument("Division by zero"); return a / b; };

    double res;
    for (auto &token : tokens)
    {

        const std::string str = token.GetToken();

        // std::cout << "cur: " << str << std::endl;

        switch (token.GetTokenType())
        {
        case TokenType::Integer:
            stack.push(std::stof(str));
            break;
        case TokenType::Float:
            stack.push(std::stof(str));
            break;
        case TokenType::Operator:
        {
            switch (token.GetAssociativity())
            {
            case OperatorAssociativity::Left:
            {
                auto [a, b] = getTwoTokens();
                if (str == "+")
                    res = a + b;
                else if (str == "-")
                    res = a - b;
                else if (str == "*")
                    res = a * b;
                else if (str == "/")
                    res = checkedDivision(a, b);
                else
                    throw std::invalid_argument("Unknown operator!");
                
                break;
            }
            case OperatorAssociativity::Right:
            {
                auto a = getOneToken();
                if (str == "-")
                    res = -a;
                else
                    throw std::invalid_argument("Unknown operator!");
                break;
            }
            case OperatorAssociativity::None:
                throw std::logic_error("Operator must have associativity!");
                break;
            }
            stack.push(res);
            break;
        }

        case TokenType::Function:
            if (str == "log")
            {
                auto [a, b] = getTwoTokens();
                if (a <= 0.f || a == 1.0f || b <= 0.f)
                    throw std::invalid_argument("log(a,x): incorrect arguments");
                res = std::log(b) / std::log(a);
            }
            else
                throw std::invalid_argument("Unknown function: " + str);
            stack.push(res);
            break;
        default:
            break;
        }
    }
    if(stack.size() > 1)
        throw std::invalid_argument("An extra argument in the function");
    return stack.top();
}