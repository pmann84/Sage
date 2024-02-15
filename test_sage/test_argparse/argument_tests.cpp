#include <sage/argparse/argparse.hpp>

#include "gtest/gtest.h"

TEST(ArgumentTests, CheckDestinationIsCorrectForPositionalArguments)
{
    std::string argument_name = "foo";
    sage::argparse::argument arg(argument_name);
    ASSERT_EQ(arg.dest(), "foo");
}

TEST(ArgumentTests, CheckDestinationIsCorrectForOptionalArguments)
{
    std::vector<std::string> argument_names = {"--myarg", "-m"};
    sage::argparse::argument arg(argument_names);
    ASSERT_EQ(arg.dest(), "myarg");
}

TEST(ArgumentTests, CheckDestinationIsCorrectForOptionalArgumentsMultipleSingleDashes)
{
    std::vector<std::string> argument_names = {"-myarg", "-m"};
    sage::argparse::argument arg(argument_names);
    ASSERT_EQ(arg.dest(), "myarg");
}

TEST(ArgumentTests, TestOptionalArgumentCanBeIdentifiedByAllNames)
{
    std::vector<std::string> argument_names = {"--myarg", "-m"};
    sage::argparse::argument arg(argument_names);
    ASSERT_TRUE(arg.matches_arg_name("-m"));
    ASSERT_TRUE(arg.matches_arg_name("--myarg"));
}