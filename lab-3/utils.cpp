#include "utils.hpp"
namespace utils
{
    Configuration::Configuration() = default;

    Configuration::Configuration(const std::string &state,
                                 const int &index,
                                 const std::stack<char> &magazine) : state(state), index(index), magazine(magazine) {}

    std::string Configuration::ToString() const
    {
        std::ostringstream oss;
        oss << state + "|" + std::to_string(index) + "|";
        std::stack<char> buffer(magazine);
        while (buffer.size())
        {
            oss << buffer.top();
            buffer.pop();
        }
        return oss.str();
    }
}
