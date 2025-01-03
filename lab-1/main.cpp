#include <iostream>

#include "Calculator.hpp"

int main(int argc, char const *argv[])
{
    std::string data;
    while (true)
    {
        try
        {
            getline(std::cin, data);
            Tokenizer tokenizer;
            std::vector<Token> tokens = tokenizer(data);

            ShuntingYard shuntingYard;
            std::vector<Token> rpn = shuntingYard(tokens);

            EvaluateRPN ev;
            std::cout << ev(rpn) << std::endl;
        }
        catch (std::invalid_argument e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    return 0;
}
