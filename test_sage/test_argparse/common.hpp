#pragma once

#include <tuple>
#include <string>
#include <vector>

inline std::tuple<int, const char**> make_args(std::initializer_list<std::string> args)
{
    const char** output = new const char*[100];
    auto i = 0;
    for(auto& arg: args)
    {
        output[i] = arg.c_str();
    }
    return std::make_tuple(args.size(), output);
}