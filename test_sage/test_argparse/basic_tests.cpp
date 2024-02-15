#include <sage/argparse/argparse.hpp>

#include "gtest/gtest.h"
#include "common.hpp"

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
    parser.add_argument("bar").help("Positional bar argument.");

    const auto [argc, argv] = make_args({"DummyApp.exe", "BAR"});
    parser.parse_args(argc, (char**)argv);
    for (int i = 0; i < argc; ++i) {
        std::cout << "Arg " << i << ": " << argv[i] << std::endl;
    }
    const auto val = parser.get<std::string>("bar");
    ASSERT_EQ(val, argv[1]);
}

TEST_F(BasicTests, TestOnePositionalArgumentNotEnteredExits)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument("bar").help("Positional bar argument.");

    const auto [argc, argv] = make_args({"DummyApp.exe"});
    EXPECT_EXIT(parser.parse_args(argc, (char**)argv), testing::ExitedWithCode(1), "");
}

TEST_F(BasicTests, TestTwoPositionalArgumentsSuccessfullyEntered)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument("bar").help("Positional bar argument.");
    parser.add_argument("foo").help("Positional foo argument.");

    const auto [argc, argv] = make_args({"DummyApp.exe", "BAR", "FOO"});
    parser.parse_args(argc, (char**)argv);
    ASSERT_EQ(parser.get<std::string>("bar"), argv[1]);
    ASSERT_EQ(parser.get<std::string>("foo"), argv[2]);
}

TEST_F(BasicTests, TestOnePositionalArgumentNotEnteredButTwoRequiredExits)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument("bar").help("Positional bar argument.");
    parser.add_argument("foo").help("Positional foo argument.");

    const auto [argc, argv] = make_args({"DummyApp.exe", "BAR"});
    EXPECT_EXIT(parser.parse_args(argc, (char**)argv), testing::ExitedWithCode(1), "");
}

TEST_F(BasicTests, TestThreePositionalArgumentEnteredButTwoRequiredExits)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument("bar").help("Positional bar argument.");
    parser.add_argument("foo").help("Positional foo argument.");

    const auto [argc, argv] = make_args({"DummyApp.exe", "FOO", "BAR", "BAZ"});
    EXPECT_EXIT(parser.parse_args(argc, (char**)argv), testing::ExitedWithCode(1), "");
}

TEST_F(BasicTests, TestOptionalArgumentEnteredSuccessfully)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument({"--foo", "-f"}).help("Optional Foo argument.");

    const auto [argc, argv] = make_args({"DummyApp.exe", "--foo", "FOO"});
    parser.parse_args(argc, (char**)argv);
    ASSERT_EQ(parser.get<std::string>("foo"), argv[2]);
}

TEST_F(BasicTests, TestOptionalArgumentShortVersionEnteredSuccessfully)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument({"--foo", "-f"}).help("Optional Foo argument.");

    const auto [argc, argv] = make_args({"DummyApp.exe", "-f", "FOO"});
    parser.parse_args(argc, (char**)argv);
    ASSERT_EQ(parser.get<std::string>("foo"), argv[2]);
}

TEST_F(BasicTests, TestPositionalAndOptionalArgumentEnteredSuccessfully)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument("bar").help("Positional bar argument.");
    parser.add_argument({"--foo", "-f"}).help("Optional Foo argument.");

    const auto [argc, argv] = make_args({"DummyApp.exe", "BAR", "-f", "FOO"});
    parser.parse_args(argc, (char**)argv);
    ASSERT_EQ(parser.get<std::string>("bar"), argv[1]);
    ASSERT_EQ(parser.get<std::string>("foo"), argv[3]);
}

TEST_F(BasicTests, TestPositionalAndOptionalArgumentEnteredSuccessfullyOptionalArgumentFirst)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    parser.add_argument("bar").help("Positional bar argument.");
    parser.add_argument({"--foo", "-f"}).help("Optional Foo argument.");

    const auto [argc, argv] = make_args({"DummyApp.exe", "-f", "FOO", "BAR"});
    parser.parse_args(argc, (char**)argv);
    ASSERT_EQ(parser.get<std::string>("bar"), argv[3]);
    ASSERT_EQ(parser.get<std::string>("foo"), argv[2]);
}

TEST_F(BasicTests, TestMixedNameArgumentsThrowError)
{
    auto parser = sage::argparse::argument_parser("MyParser", "Commandline options for my application!");
    ASSERT_ANY_THROW(parser.add_argument({"foo", "-f"}));
}