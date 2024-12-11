#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>

#include "DFA.hpp"
#include "NFA.hpp"

namespace utils
{
    std::string SetToString(const std::set<std::string> &st)
    {
        if (st.empty())
            return "";
        if (st.size() == 1)
            return *st.begin();
        std::string answer;
        answer.append("{" + *st.begin());
        for (auto it = ++st.begin(); it != --st.end(); it++)
            answer.append(", " + *it);
        answer.append(", " + *(--st.end()) + "}");
        return answer;
    }

    DFA ConvertToDFA(const NFA &nfa)
    {
        std::unordered_set<std::string> states{nfa.GetInitialState()};
        std::unordered_set<std::string> finalStates;
        dfa_transitions transitions;

        std::queue<std::set<std::string>> q;
        q.push({nfa.GetInitialState()});
        while (q.size())
        {
            std::set<std::string> currentStates = q.front();
            q.pop();
            for (const char &symbol : nfa.GetAlphabet())
            {
                std::set<std::string> tempSet;
                for (const std::string &state : currentStates)
                    tempSet.insert(nfa.GetTransitions()[state][symbol].begin(),
                                   nfa.GetTransitions()[state][symbol].end());
                if (tempSet.empty())
                    continue;
                std::string tempState = SetToString(tempSet);

                transitions[SetToString(currentStates)][symbol] = tempState;

                for (const std::string &check : tempSet)
                {
                    if (nfa.IsFinalState(check))
                    {
                        finalStates.insert(tempState);
                        break;
                    }
                }

                if (states.find(tempState) != end(states))
                    continue;
                states.insert(tempState);
                q.push(tempSet);
            }
        }
        return DFA(states, nfa.GetAlphabet(), "0", finalStates, transitions);
    }
}