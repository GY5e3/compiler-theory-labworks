#pragma once

#include <unordered_map>
#include <memory>
#include <sstream>

#include "AbstractAutomaton.hpp"

using dfa_transitions = std::unordered_map<std::string, std::unordered_map<char, std::string>>;

class DFA : public AbstractAutomaton
{
public:
    DFA() = default;
    DFA(const std::unordered_set<std::string> &states,
        const std::unordered_set<char> &alphabet,
        const std::string &initialState,
        const std::unordered_set<std::string> &finalStates,
        const dfa_transitions &transitions) : AbstractAutomaton(states, alphabet, initialState, finalStates),
                                              m_transitions(transitions) {}

    dfa_transitions GetTransitions() const;

    bool Accept(const std::string &word) const override;
    std::string ToString() const override;
private:
    dfa_transitions m_transitions;
};