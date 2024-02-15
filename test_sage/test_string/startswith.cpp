#include <sage/string/utilities.hpp>

#include "gtest/gtest.h"

TEST(StringStartsWith, TestStringStartsWith)
{
   const std::string str_to_test("Hello my name is stringy!");
   ASSERT_TRUE(sage::string::utilities::starts_with(str_to_test, std::string("Hello")));
}

TEST(StringStartsWith, TestPrefixNotInString)
{
   const std::string str_to_test("Hello my name is stringy!");
   ASSERT_FALSE(sage::string::utilities::starts_with(str_to_test, std::string("Blarg")));
}

TEST(StringStartsWith, TestPrefixInStringButNotAtStart)
{
   const std::string str_to_test("Hello my name is stringy!");
   ASSERT_FALSE(sage::string::utilities::starts_with(str_to_test, std::string("is")));
}

TEST(StringStartsWith, TestStringStartsWithWstring)
{
   const std::wstring str_to_test(L"Hello my name is stringy!");
   ASSERT_FALSE(sage::string::utilities::starts_with(str_to_test, std::wstring(L"is")));
}

TEST(StringStartsWith, TestEmptyString)
{
   const std::wstring str_to_test(L"");
   ASSERT_FALSE(sage::string::utilities::starts_with(str_to_test, std::wstring(L"stringy!")));
}