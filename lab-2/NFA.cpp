#include "NFA.hpp"

nfa_transitions NFA::GetTransitions() const
{
    return m_transitions;
}
// Stub, not working now
bool NFA::Accept(const std::string &word) const
{
    return false;
}

std::string NFA::ToString() const
{
    std::ostringstream oss;

    for (auto state : GetStates())
    {
        oss << "State " + state + ":" << std::endl;
        if (m_transitions.find(state) != end(m_transitions))
        {
            for (const auto &transition : m_transitions.find(state)->second)
            {
                oss << "  \"" << transition.first << "\" --> ";

                for (const std::string &nextStates : transition.second)
                    oss << nextStates << " ";
                oss << std::endl;
            }
        }
    }
    return oss.str();
}