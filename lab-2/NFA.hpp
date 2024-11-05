#pragma once

#include <unordered_map>
#include <sstream>

#include "AbstractAutomaton.hpp"

using nfa_transitions = std::unordered_map<std::string, std::unordered_map<char, std::unordered_set<std::string>>>;

class NFA : public AbstractAutomaton
{
public:
    NFA() = default;
    NFA(const std::unordered_set<std::string> &states,
        const std::unordered_set<char> &alphabet,
        const std::string &initialState,
        const std::unordered_set<std::string> &finalStates,
        const nfa_transitions &transitions) : AbstractAutomaton(states, alphabet, initialState, finalStates),
                                              m_transitions(transitions) {}

    nfa_transitions GetTransitions() const;

    bool Accept(const std::string &word) const override;
    std::string ToString() const override;

private:
    std::unordered_set<std::string> m_states;
    std::unordered_set<char> m_alphabet;
    std::string m_initialState;
    std::unordered_set<std::string> m_finalStates;
    nfa_transitions m_transitions;
};