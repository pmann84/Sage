#include <sage/argparse/argparse.hpp>

#include "common_setup.hpp"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

TEST(DefaultArgumentTests, TestDefaultOptionalArgumentReturnedAndNoErrorWhenArgumentNotSubmitted)
{
    const std::string default_arg_value = "my BAR";
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser
        .add_argument("--bar")
        .help("Optional bar argument.")
        .default_value(default_arg_value);

    std::vector<char*> argv = {AppName};
    parser.parse_args((int)argv.size(), &argv[0]);
    ASSERT_EQ(parser.get<std::string>("bar"), default_arg_value);
}

TEST(DefaultArgumentTests, TestDefaultOptionalArgumentReturnedForConsumeAllIsAListWithASingleValue)
{
    const std::string default_arg_value = "my BAR";
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser
        .add_argument("bar")
        .num_args("*")
        .help("Positional bar argument.")
        .default_value(default_arg_value);

    std::vector<char*> argv = {AppName};
    parser.parse_args((int)argv.size(), &argv[0]);
    ASSERT_THAT(parser.get<std::vector<std::string>>("bar"), ::testing::ContainerEq(std::vector<std::string>({default_arg_value})));
}