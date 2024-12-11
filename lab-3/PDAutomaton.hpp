#pragma once

#include <iostream>
#include <sstream>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "utils.hpp"

using grammar_rules = std::unordered_map<char, std::unordered_set<std::string>>;
using pda_transitions = std::unordered_map<std::tuple<std::string, char, char>, std::unordered_set<std::tuple<std::string, std::string>, utils::TupleHash>, utils::TupleHash>;

class PDAutomaton
{
public:
    PDAutomaton();
    PDAutomaton(const std::string &initialState,
                const char &initialMagazineNonerminal,
                const std::unordered_set<std::string> &states,
                const std::unordered_set<std::string> &finalStates,
                const std::unordered_set<char> &terminals,
                const std::unordered_set<char> &nonterminals,
                const grammar_rules &transitions);
    bool Accept(const std::string &word, std::vector<std::string> &configChain) const;
    std::string ToString() const;

private:
    std::string m_initialState;
    char m_initialMagazineNonerminal;
    std::unordered_set<std::string> m_states;
    std::unordered_set<std::string> m_finalStates;
    std::unordered_set<char> m_terminals;
    std::unordered_set<char> m_nonterminals;
    pda_transitions m_transitions;
};