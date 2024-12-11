#pragma once

#include <algorithm>
#include <stack>
#include <sstream>
#include <tuple>

namespace utils
{
    struct TupleHash
    {
        template <typename... T>
        size_t operator()(const std::tuple<T...> &tuple) const
        {
            return std::apply([](const auto &...args)
                              {
            size_t seed = 0;
            (..., (seed ^= std::hash<std::decay_t<decltype(args)>>{}(args) + 0x9e3779b9 + (seed << 6) + (seed >> 2)));
            return seed; }, tuple);
        }
    };
    struct Configuration
    {
    public:
        std::string state;
        int index;
        std::stack<char> magazine;

        Configuration();
        Configuration(const std::string &state,
                      const int &index,
                      const std::stack<char> &magazine);
        std::string ToString() const;
    };
}
