#include "DFA.hpp"

dfa_transitions DFA::GetTransitions() const
{
    return m_transitions;
}
bool DFA::Accept(const std::string &word) const
{
    std::string currentState = m_initialState;

    for (const char &symbol : word)
    {
        auto temp = std::make_shared<std::unordered_map<char, std::string>>(m_transitions.find(currentState)->second);
        if (temp->find(symbol) == end(*temp))
            return false;

        currentState = temp->find(symbol)->second;
    }
    return IsFinalState(currentState);
}
std::string DFA::ToString() const
{
    std::ostringstream oss;

    for (const std::string &state : m_states)
    {
        oss << "State " + state + ":" << std::endl;
        if (m_transitions.find(state) != end(m_transitions))
        {
            for (auto transition : m_transitions.find(state)->second)
            {
                oss << "  \"" << transition.first << "\" --> " + transition.second << std::endl;
            }
        }
    }
    return oss.str();
}