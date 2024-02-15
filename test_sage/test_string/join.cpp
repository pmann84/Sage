#include <sage/string/utilities.hpp>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

TEST(JoinString, TestStringJoinsCorrectly)
{
   const std::vector<std::string> strings_to_join = { "Hello", "my", "name", "is", "stringy!" };
   const std::string joined_string = sage::string::utilities::join(strings_to_join, std::string(" "));
   ASSERT_EQ(joined_string, "Hello my name is stringy!");
}

TEST(JoinString, TestStringJoinsCorrectlyWhenDelimiterIsMultipleCharacters)
{
   const std::vector<std::string> strings_to_join = { "Hello","lit", "ring" };
   const std::string joined_string = sage::string::utilities::join(strings_to_join, std::string("sp"));
   ASSERT_THAT(joined_string, "Hellosplitspring");
}

TEST(JoinString, TestStringJoinCorrectlyWideString)
{
   const std::vector<std::wstring> strings_to_join = { L"Hello", L"my", L"name", L"is", L"stringy!" };
   const std::wstring joined_string = sage::string::utilities::join(strings_to_join, std::wstring(L" "));
   ASSERT_EQ(joined_string, L"Hello my name is stringy!");
}

TEST(JoinString, TestStringJoinWithEmptyStrings)
{
   const std::vector<std::wstring> strings_to_join = { L"", L"", L"", L"is", L"stringy!" };
   const std::wstring joined_string = sage::string::utilities::join(strings_to_join, std::wstring(L" "));
   ASSERT_EQ(joined_string, L"   is stringy!");
}

TEST(JoinString, TestStringJoinEmptyString)
{
   const std::vector<std::wstring> strings_to_join = {};
   const std::wstring joined_string = sage::string::utilities::join(strings_to_join, std::wstring(L" "));
   ASSERT_EQ(joined_string, L"");
}

TEST(JoinString, TestStringJoinWithOneElement)
{
   const std::vector<std::wstring> strings_to_join = { L"Hello" };
   const std::wstring joined_string = sage::string::utilities::join(strings_to_join, std::wstring(L" "));
   ASSERT_EQ(joined_string, L"Hello");
}