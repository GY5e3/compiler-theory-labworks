#pragma once

#include <iostream>
#include <unordered_set>

class AbstractAutomaton
{
public:
    AbstractAutomaton() = default;
    AbstractAutomaton(const std::unordered_set<std::string> &states,
                       const std::unordered_set<char> &alphabet,
                       const std::string &initialState,
                       const std::unordered_set<std::string> &finalStates) : m_states(states),
                                                                             m_alphabet(alphabet),
                                                                             m_initialState(initialState),
                                                                             m_finalStates(finalStates)
    {
        if(m_finalStates.empty())
            throw std::invalid_argument("FSA must have at least 1 final state");
        if(states.find(initialState) == end(states))
            throw std::invalid_argument("The initial state is missing in the set of FSA states");

    }
    virtual bool IsFinalState(const std::string &state) const
    {
        return m_finalStates.find(state) != end(m_finalStates);
    }
    virtual std::unordered_set<std::string> GetStates() const { return m_states; }
    virtual std::unordered_set<char> GetAlphabet() const { return m_alphabet; }
    virtual std::string GetInitialState() const { return m_initialState; }
    virtual std::unordered_set<std::string> GetFinalStates() const { return m_finalStates; }

    virtual bool Accept(const std::string &word) const = 0;
    virtual std::string ToString() const = 0;

protected:
    std::unordered_set<std::string> m_states;
    std::unordered_set<char> m_alphabet;
    std::string m_initialState;
    std::unordered_set<std::string> m_finalStates;
};