#include <sage/argparse/argparse.hpp>

#include "common_setup.hpp"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

TEST(NargsTests, TestArgumentsAreReturnedForSuccessfullyEnteredMultiplePositionalArguments)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument("foo").num_args(3).help("Positional foo argument.");

    std::vector<char*> argv = {AppName, FooArgValue1, FooArgValue2, FooArgValue3};
    parser.parse_args((int)argv.size(), &argv[0]);

    ASSERT_THAT(parser.get<std::vector<std::string>>("foo"), ::testing::ContainerEq(std::vector<std::string>({FooArgValue1, FooArgValue2, FooArgValue3})));
}

TEST(NargsTests, TestExitsWhenInsufficientMultiplePositionalArgumentsAreEntered)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument(FooArgName).num_args(3).help(FooHelp);

    std::vector<char*> argv = {AppName, FooArgValue1, FooArgValue2};
    EXPECT_EXIT(parser.parse_args((int)argv.size(), &argv[0]), testing::ExitedWithCode(1), "");
}

TEST(NargsTests, TestMultipleArgumentsAreReturnedForSuccessfullyEnteredMultiplePositionalArguments)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument(FooArgName).num_args(3).help(FooHelp);
    parser.add_argument(BarArgName).num_args(2).help(BarHelp);

    std::vector<char*> argv = {AppName, FooArgValue1, FooArgValue2, FooArgValue3, BarArgValue1, BarArgValue2};
    parser.parse_args((int)argv.size(), &argv[0]);

    ASSERT_THAT(parser.get<std::vector<std::string>>("foo"), ::testing::ContainerEq(std::vector<std::string>({"FOO1", "FOO2", "FOO3"})));
    ASSERT_THAT(parser.get<std::vector<std::string>>("bar"), ::testing::ContainerEq(std::vector<std::string>({"BAR1", "BAR2"})));
}

TEST(NargsTests, TestExitsWhenInsufficientArgumentsAreEnteredMultiplePositionalArguments)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument(FooArgName).num_args(3).help(FooHelp);
    parser.add_argument(BarArgName).num_args(2).help(BarHelp);

    std::vector<char*> argv = {AppName, FooArgValue1, FooArgValue2, BarArgValue1};

    EXPECT_EXIT(parser.parse_args((int)argv.size(), &argv[0]), testing::ExitedWithCode(1), "");
}

TEST(NargsTests, TestAllPositionalArgsAreConsumedWhenOnlyPositionalArgumentsPresent)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument(FooArgName).num_args("*").help("Positional bar argument consumes all.");

    std::vector<char*> argv = {AppName, FooArgValue1, FooArgValue2, FooArgValue3, FooArgValue4, FooArgValue5};
    parser.parse_args((int)argv.size(), &argv[0]);
    ASSERT_THAT(parser.get<std::vector<std::string>>("foo"), ::testing::ContainerEq(std::vector<std::string>({FooArgValue1, FooArgValue2, FooArgValue3, FooArgValue4, FooArgValue5})));
}

TEST(NargsTests, TestEmptyVectorReturnedWhenConsumingAllArgumentsButNoInputsGiven)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument(FooArgName).num_args("*").help("Positional bar argument consumes all.");

    std::vector<char*> argv = {AppName};
    parser.parse_args((int)argv.size(), &argv[0]);
    ASSERT_TRUE(parser.get<std::vector<std::string>>("foo").empty());
}

TEST(NargsTests, TestAllPositionalArgsAreConsumedUpToNextOptionalArgument)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument(FooArgName).num_args("*").help("Positional bar argument consumes all.");
    parser.add_argument({BarShortOptArgName, BarOptArgName}).help("Optional bar argument.");

    std::vector<char*> argv = {AppName, FooArgValue1, FooArgValue2, FooArgValue3, FooArgValue4, FooArgValue5, BarShortOptArgName, BarArgValue};
    parser.parse_args((int)argv.size(), &argv[0]);
    ASSERT_THAT(parser.get<std::vector<std::string>>("foo"), ::testing::ContainerEq(std::vector<std::string>({FooArgValue1, FooArgValue2, FooArgValue3, FooArgValue4, FooArgValue5})));
    ASSERT_EQ(parser.get<std::string>(BarArgName), BarArgValue);
}

TEST(NargsTests, TestAllPositionalArgsAreConsumedWithMultiplePositionalArguments)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument("foo").num_args("*").help("Positional bar argument consumes all.");
    parser.add_argument("bar").num_args("*").help("Positional bar argument consumes all too!");

    std::vector<char*> argv = {AppName, FooArgValue1, FooArgValue2, FooArgValue3, FooArgValue4, FooArgValue5};
    parser.parse_args((int)argv.size(), &argv[0]);

    ASSERT_THAT(parser.get<std::vector<std::string>>("foo"), ::testing::ContainerEq(std::vector<std::string>({"FOO1", "FOO2", "FOO3", "FOO4", "FOO5"})));
    ASSERT_TRUE(parser.get<std::string>("bar").empty());
}

TEST(NargsTests, TestPositionalArgumentRecievedGivingArgumentsAfterConsumeAllPositionalAndOptionalArgs)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument("foo").num_args("*").help("Positional bar argument consumes all.");
    parser.add_argument({"-b", "--bar"}).help("Optional bar argument.");

    std::vector<char*> argv = {AppName, FooArgValue1, FooArgValue2, FooArgValue3, FooArgValue4, FooArgValue5, BarShortOptArgName, BarArgValue};
    parser.parse_args((int)argv.size(), &argv[0]);

    ASSERT_THAT(parser.get<std::vector<std::string>>("foo"), ::testing::ContainerEq(std::vector<std::string>({"FOO1", "FOO2", "FOO3", "FOO4", "FOO5"})));
    ASSERT_THAT(parser.get<std::vector<std::string>>("bar"), ::testing::ContainerEq(std::vector<std::string>({"BAR"})));
}

TEST(NargsTests, TestUnknownPositionalArgumentRecievedGivingArgumentsAfterConsumeAllPositionalAndOptionalArgs)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument("foo").num_args("*").help("Positional bar argument consumes all.");
    parser.add_argument({"-b", "--bar"}).help("Optional bar argument.");

    std::vector<char*> argv = {AppName, FooArgValue1, FooArgValue2, FooArgValue3, FooArgValue4, FooArgValue5, BarShortOptArgName, BarArgValue, BarArgValue1};

    EXPECT_EXIT(parser.parse_args((int)argv.size(), &argv[0]), testing::ExitedWithCode(1), "");
}

TEST(NargsTests, TestAsManyPositionalArgumentsAreConsumedThenAllTheRestAreConsumedAsOptional)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument("foo").num_args("*").help("Positional bar argument consumes all.");
    parser.add_argument({"-b", "--bar"}).num_args("*").help("Optional bar argument consumes all.");

    std::vector<char*> argv = {AppName, FooArgValue1, FooArgValue2, FooArgValue3, FooArgValue4, FooArgValue5, BarShortOptArgName, BarArgValue, BarArgValue1};

    parser.parse_args((int)argv.size(), &argv[0]);

    ASSERT_THAT(parser.get<std::vector<std::string>>("foo"), ::testing::ContainerEq(std::vector<std::string>({"FOO1", "FOO2", "FOO3", "FOO4", "FOO5"})));
    ASSERT_THAT(parser.get<std::vector<std::string>>("bar"), ::testing::ContainerEq(std::vector<std::string>({"BAR", "BAR1"})));
}

TEST(NargsTests, TestAsManyPositionalArgumentsAreConsumedThenNoneConsumedWhenNoOptionalArgumentsPresent)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument("foo").num_args("*").help("Positional bar argument consumes all.");
    parser.add_argument({"-b", "--bar"}).num_args("*").help("Optional bar argument consumes all.");

    std::vector<char*> argv = {AppName, FooArgValue1, FooArgValue2, FooArgValue3, FooArgValue4, FooArgValue5, BarShortOptArgName};

    parser.parse_args((int)argv.size(), &argv[0]);
    
    ASSERT_THAT(parser.get<std::vector<std::string>>("foo"), ::testing::ContainerEq(std::vector<std::string>({"FOO1", "FOO2", "FOO3", "FOO4", "FOO5"})));
    ASSERT_TRUE(parser.get<std::vector<std::string>>("bar").empty());
}

TEST(NargsTests, TestSingleArgumentReturnsCorrectValueForPresentPositionalArgument)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument("foo").num_args("?").help("Positional bar argument consumes single if available.");

    std::vector<char*> argv = {AppName, FooArgValue1};
    parser.parse_args((int)argv.size(), &argv[0]);
    ASSERT_EQ(argv[1], parser.get<std::string>("foo"));
}

TEST(NargsTests, TestSingleArgumentReturnsCorrectDefaultValueForPositionalArgumentWithNoValueProvidedAndNoDefaultProvided)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument("foo").num_args("?").help("Positional bar argument consumes single if available.");

    std::vector<char*> argv = {AppName};
    parser.parse_args((int)argv.size(), &argv[0]);
    ASSERT_EQ("", parser.get<std::string>("foo"));
}

TEST(NargsTests, TestSingleArgumentReturnsCorrectDefaultValueForPositionalArgumentWithNoValueProvided)
{
    const std::string default_arg_value = "my FOO";
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument("foo").num_args("?").default_value(default_arg_value).help("Positional bar argument consumes single if available.");

    std::vector<char*> argv = {AppName};
    parser.parse_args((int)argv.size(), &argv[0]);
    ASSERT_EQ(default_arg_value, parser.get<std::string>("foo"));
}


// Single arg Test cases

// Note that ? args only consume if there is an arg available to consume, other args take precedence in consuming arguments

// foo = ?, bar (1 arg) -> parse(["hello"]) => bar = hello, foo = default/none
// foo = ?, bar (1 arg) -> parse(["hello", "world"]) => bar = world, foo = hello
// bar (1 arg), foo = ? -> parse(["hello", "world"]) => bar = hello, foo = world (respects order)
// foo = ? -> parse(["hello", "world"]) => unrecognized arg "world"
// foo = ?, bar = ? -> parse(["hello", "world", "again"]) => unrecognized arg "again"


// At Least One Test Cases
// foo = + => parse([]) => Error generated - at least one arg expected
// foo = + => parse(["Hello"]) => foo = Hello
// foo = + => parse(["Hello", "World"]) => foo = [Hello, World]

// TODO: test integer nargs with optional arguments
// TODO: Test switches with narg 0