#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "DFA.hpp"
#include "NFA.hpp"
#include "utils.hpp"

NFA ReadNFAfromFile(const std::string &filePath);

int main(int argc, char const *argv[])
{
    NFA nfa = ReadNFAfromFile("_data.txt");
    DFA dfa = utils::ConvertToDFA(nfa);

    std::cout << "Before determination:\n" + nfa.ToString();
    std::cout << "After determination:\n" + dfa.ToString();

    std::string data;
    while (true)
    {
        try
        {
            getline(std::cin, data);
            if(dfa.Accept(data)) {
                std::cout << "Success!" << std::endl;
            }
            else {
                std::cout << "Fail!" << std::endl;
            }
        }
        catch (std::invalid_argument e)
        {
            std::cout << e.what() << std::endl;
        }
    }


    return 0;
}
/*
Laboratory work condition:
Read the transition graph in the format q<N>,<C>=<q|f><N>
where q - state, f - final state, N - integer, C - ONE symbol of transition
*/
NFA ReadNFAfromFile(const std::string &filePath)
{
    std::ifstream file("_data.txt");
    if (!file.is_open())
        throw std::invalid_argument("File not found");

    std::unordered_set<std::string> states;
    std::unordered_set<char> alphabet;
    std::unordered_set<std::string> finalStates;
    std::unordered_map<std::string, std::unordered_map<char, std::unordered_set<std::string>>> transitions;

    std::string line;
    while (getline(file, line))
    {
        int index = 0;

        if (line[index++] != 'q')
            throw std::invalid_argument("Incorrect line format: " + line + "\n");

        std::string currentState = "";
        while (index < line.size() && line[index] != ',')
        {
            if ('0' > line[index] || line[index] > '9')
                throw std::invalid_argument("Incorrect transition: " + line + "\nState must be integer\n");
            currentState.push_back(line[index++]);
        }
        if (currentState.empty())
            throw std::invalid_argument("Incorrect transition: " + line + "\nState must be integer\n");

        if (++index >= line.size())
            throw std::invalid_argument("Incorrect line format: " + line + "\n");

        char symbolOfTransition = line[index++];

        if (index >= line.size() || line[index++] != '=')
            throw std::invalid_argument("Incorrect line format: " + line + "\n");

        if (index >= line.size() || line[index] != 'q' && line[index] != 'f')
            throw std::invalid_argument("Incorrect line format: " + line + "\n");
        bool isFinalState = line[index++] == 'f';

        std::string nextState;
        while (index < line.size())
        {
            if ('0' > line[index] || line[index] > '9')
                throw std::invalid_argument("Incorrect transition: " + line + "\nState must be integer\n");
            nextState.push_back(line[index++]);
        }

        if (nextState.empty())
            throw std::invalid_argument("Incorrect transition: " + line + "\nState must be integer\n");

        states.insert({currentState, nextState});
        alphabet.insert(symbolOfTransition);
        if (isFinalState)
            finalStates.insert(nextState);
        transitions[currentState][symbolOfTransition].insert(nextState);
    }
    file.close();
    return NFA(states, alphabet, "0", finalStates, transitions);
}
/*
q0,a=f1
q0,a=q2
q0,b=q2
q2,o=q2
q2,o=f1
*/