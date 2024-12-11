#include "PDAutomaton.hpp"

PDAutomaton::PDAutomaton() = default;
PDAutomaton::PDAutomaton(const std::string &initialState,
                         const char &initialMagazineNonerminal,
                         const std::unordered_set<std::string> &states,
                         const std::unordered_set<std::string> &finalStates,
                         const std::unordered_set<char> &terminals,
                         const std::unordered_set<char> &nonterminals,
                         const grammar_rules &transitions) : m_initialState(initialState),
                                                             m_initialMagazineNonerminal(initialMagazineNonerminal),
                                                             m_states(states),
                                                             m_finalStates(finalStates),
                                                             m_terminals(terminals),
                                                             m_nonterminals(nonterminals)
{
    if (nonterminals.find(initialMagazineNonerminal) == end(nonterminals))
        throw std::invalid_argument("The initial magazine nonterminal is missing in the set of nonterminals");
    if (states.find(initialState) == end(states))
        throw std::invalid_argument("The initial state is missing in the set of FSA states");

    for (const std::string &state : states)
    {
        for (const char &terminal : m_terminals)
            m_transitions[{state, terminal, terminal}].insert({state, "\0"});

        for (const char &nonterminal : m_nonterminals)
        {
            auto targets = transitions.find(nonterminal);
            if (targets == end(transitions))
                continue;
            for (const std::string &expression : targets->second)
                m_transitions[{state, '\0', nonterminal}].insert({state, expression});
        }
        for (const std::string &finalState : finalStates)
            m_transitions[{state, '\0', '\xFF'}].insert({finalState, "\0"});
    }
}
bool PDAutomaton::Accept(const std::string &word, std::vector<std::string> &configChain) const
{
    std::unordered_set<std::string> visited;
    std::queue<std::pair<utils::Configuration, int>> q;
    utils::Configuration configDefault{m_initialState, 0, std::stack<char>({'\xFF', m_initialMagazineNonerminal})};
    q.push({configDefault, -1});
    std::vector<std::pair<utils::Configuration, int>> configTree;

    while (q.size())
    {
        auto buildConfigChain = [&]()
        {
            int index = configTree.size() - 1;
            while (index)
            {
                configChain.push_back(configTree[index].first.ToString());
                index = configTree[index].second;
            }
        };
        configTree.push_back(q.front());
        utils::Configuration currentConfig = std::move(q.front().first);
        char strSymbol = currentConfig.index < word.size() ? word[currentConfig.index] : '\0';
        q.pop();

        if (m_finalStates.find(currentConfig.state) != end(m_finalStates) &&
            strSymbol == '\0' &&
            currentConfig.magazine.empty())
        {
            buildConfigChain();
            return true;
        }

        if (currentConfig.magazine.size() > word.size() + 1 ||
            strSymbol != '\0' && (currentConfig.magazine.size() == 1 || m_terminals.find(strSymbol) == end(m_terminals)))
            continue;

        auto terminalTransitions = m_transitions.find({currentConfig.state, strSymbol, currentConfig.magazine.top()});
        auto nonterminalTransitions = m_transitions.find({currentConfig.state, '\0', currentConfig.magazine.top()});

        auto nextConfigurations = [&](const pda_transitions::const_iterator &transitions)
        {
            if (transitions == end(m_transitions))
                return;

            for (const std::tuple<std::string, std::string> &next : transitions->second)
            {
                std::stack<char> nextMagazine(currentConfig.magazine);
                nextMagazine.pop();
                for (size_t i = 0; i < std::get<1>(next).size(); i++)
                    nextMagazine.push(std::get<1>(next)[i]);

                utils::Configuration nextConfig(std::get<0>(next),
                                                currentConfig.index + (std::get<1>(next).empty()),
                                                std::move(nextMagazine));
                std::string nextConfigStr = nextConfig.ToString();
                if (visited.find(nextConfigStr) == end(visited))
                {
                    q.push({std::move(nextConfig), configTree.size() - 1});
                    visited.insert(std::move(nextConfigStr));
                }
            }
        };

        nextConfigurations(terminalTransitions);
        nextConfigurations(nonterminalTransitions);
    }
    return false;
}
std::string PDAutomaton::ToString() const
{
    std::ostringstream oss;
    for (auto i : m_transitions)
    {
        oss << "δ(" << std::get<0>(i.first) + ", ";
        oss << (std::get<1>(i.first) != '\0' ? std::string(1, std::get<1>(i.first)) : "λ") << ", ";
        oss << (std::get<2>(i.first) != '\xFF' ? std::string(1, std::get<2>(i.first)) : "h0") << ") = { ";

        for (auto j : i.second)
        {
            oss << "(" + std::get<0>(j) << ", ";
            oss << (std::get<1>(j) != "\0" ? std::get<1>(j) : "λ") << ") ";
        }
        oss << "}" << std::endl;
    }
    return oss.str();
}