#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "utils.hpp"
#include "PDAutomaton.hpp"

PDAutomaton ParseGrammar(const std::string&);

int main(int argc, char const *argv[])
{
    std::string filePath = "_data.txt";
    PDAutomaton pda = ParseGrammar(filePath);
    std::cout << pda.ToString();

    while (true)
    {
        std::string word;
        std::cin >> word;
        std::vector<std::string> configChain;
        if (pda.Accept(word, configChain))
        {
            std::cout << "Success!" << std::endl;
            std::reverse(configChain.begin(), configChain.end());
            for (auto i : configChain)
                std::cout << "  " + i << std::endl;
        }
        else
            std::cout << "Fail!" << std::endl;
    }

    return 0;
}
PDAutomaton ParseGrammar(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
        throw std::invalid_argument("File not found");

    std::string m_initialState = "s0";
    char m_initialMagazineNonerminal = 'E';
    std::unordered_set<std::string> m_states{"s0"};
    std::unordered_set<std::string> m_finalStates{"s0"};
    std::unordered_set<char> m_terminals;
    std::unordered_set<char> m_nonterminals;
    grammar_rules m_transitions;

    std::string line;
    while (getline(file, line))
    {
        int index = 0;
        char inputNonterminal = '\0';
        while (index < line.size() && line[index] != '>')
        {
            if (line[index] == ' ')
                continue;
            else if ('A' <= line[index] && line[index] <= 'Z' && inputNonterminal == '\0')
            {
                inputNonterminal = line[index];
                m_nonterminals.insert(inputNonterminal);
            }
            else
                throw std::invalid_argument("Incorrect line format");

            index++;
        }
        if (index++ == line.size())
            throw std::invalid_argument("Incorrect line format");

        std::string outputExpression;
        while (index < line.size())
        {
            if (line[index] == ' ')
                continue;
            else if (line[index] == '|')
            {
                std::reverse(outputExpression.begin(), outputExpression.end());
                m_transitions[inputNonterminal].insert(std::move(outputExpression));
                outputExpression = "";
            }
            else
            {
                outputExpression.push_back(line[index] != '~' ? line[index] : ' ');

                if ('A' <= line[index] && line[index] <= 'Z')
                    m_nonterminals.insert(line[index]);
                else
                    m_terminals.insert(line[index]);
            }
            index++;
        }
        std::reverse(outputExpression.begin(), outputExpression.end());
        m_transitions[inputNonterminal].insert(std::move(outputExpression));
    }
    return PDAutomaton(m_initialState, m_initialMagazineNonerminal, m_states, m_finalStates, m_terminals, m_nonterminals, m_transitions);
}
