#include <sage/argparse/argparse.hpp>

#include "common_setup.hpp"

#include "gtest/gtest.h"

class BasicTests: public ::testing::Test
{
public:
    BasicTests(): m_original_buffer() {}

    void SetUp() override
    {
        m_original_buffer = std::cout.rdbuf(nullptr);
    }

    void TearDown() override
    {
        std::cout.rdbuf(m_original_buffer);
    }

private:
    std::basic_streambuf<char, std::char_traits<char>>* m_original_buffer;
};

TEST_F(BasicTests, TestOnePositionalArgumentSuccessfullyEntered)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument(BarArgName).help(BarHelp);

    std::vector<char*> argv = {AppName, BarArgValue};
    parser.parse_args((int)argv.size(), &argv[0]);
    const auto val = parser.get<std::string>(BarArgName);
    ASSERT_EQ(val, argv[1]);
}

TEST_F(BasicTests, TestOnePositionalArgumentNotEnteredExits)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument(BarArgName).help(BarHelp);

    std::vector<char*> argv = {AppName};
    EXPECT_EXIT(parser.parse_args((int)argv.size(), &argv[0]), testing::ExitedWithCode(1), "");
}

TEST_F(BasicTests, TestTwoPositionalArgumentsSuccessfullyEntered)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument(BarArgName).help(BarHelp);
    parser.add_argument(FooArgName).help(FooHelp);

    std::vector<char*> argv = {AppName, BarArgValue, FooArgValue};
    parser.parse_args((int)argv.size(), &argv[0]);
    ASSERT_EQ(parser.get<std::string>(BarArgName), argv[1]);
    ASSERT_EQ(parser.get<std::string>(FooArgName), argv[2]);
}

TEST_F(BasicTests, TestOnePositionalArgumentNotEnteredButTwoRequiredExits)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument(BarArgName).help(BarHelp);
    parser.add_argument(FooArgName).help(FooHelp);

    std::vector<char*> argv = {AppName, BarArgValue};
    EXPECT_EXIT(parser.parse_args(argv.size(), &argv[0]), testing::ExitedWithCode(1), "");
}

TEST_F(BasicTests, TestThreePositionalArgumentEnteredButTwoRequiredExits)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument(BarArgName).help(BarHelp);
    parser.add_argument(FooArgName).help(FooHelp);

    std::vector<char*> argv = {AppName, FooArgValue, BarArgValue, BazArgValue};
    EXPECT_EXIT(parser.parse_args(argv.size(), &argv[0]), testing::ExitedWithCode(1), "");
}

TEST_F(BasicTests, TestOptionalArgumentEnteredSuccessfully)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument({FooOptArgName, FooShortOptArgName}).help("Optional Foo argument.");

    std::vector<char*> argv = {AppName, FooOptArgName, FooArgValue};
    parser.parse_args((int)argv.size(), &argv[0]);
    ASSERT_EQ(parser.get<std::string>(FooArgName), argv[2]);
}

TEST_F(BasicTests, TestOptionalArgumentShortVersionEnteredSuccessfully)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument({FooOptArgName, FooShortOptArgName}).help(FooOptHelp);

    std::vector<char*> argv = {AppName, FooShortOptArgName, FooArgValue};
    parser.parse_args((int)argv.size(), &argv[0]);
    ASSERT_EQ(parser.get<std::string>(FooArgName), argv[2]);
}

TEST_F(BasicTests, TestPositionalAndOptionalArgumentEnteredSuccessfully)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument(BarArgName).help(BarHelp);
    parser.add_argument({"--foo", "-f"}).help(FooOptHelp);

    std::vector<char*> argv = {AppName, BarArgValue, FooShortOptArgName, FooArgValue};
    parser.parse_args((int)argv.size(), &argv[0]);
    ASSERT_EQ(parser.get<std::string>("bar"), argv[1]);
    ASSERT_EQ(parser.get<std::string>("foo"), argv[3]);
}

TEST_F(BasicTests, TestPositionalAndOptionalArgumentEnteredSuccessfullyOptionalArgumentFirst)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument(BarArgName).help(BarHelp);
    parser.add_argument({FooOptArgName, FooShortOptArgName}).help(FooOptHelp);

    std::vector<char*> argv = {AppName, FooShortOptArgName, FooArgValue, BarArgValue};
    parser.parse_args((int)argv.size(), &argv[0]);
    ASSERT_EQ(parser.get<std::string>(BarArgName), argv[3]);
    ASSERT_EQ(parser.get<std::string>(FooArgName), argv[2]);
}

TEST_F(BasicTests, TestMixedNameArgumentsThrowError)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    ASSERT_ANY_THROW(parser.add_argument({"foo", "-f"}));
}