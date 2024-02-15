#include <sage/string/utilities.hpp>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

TEST(SplitString, TestStringSplitCorrectly)
{
   const std::string string_to_split("Hello my name is stringy!");
   const std::vector<std::string> split_string = sage::string::utilities::split(string_to_split, std::string(" "));
   ASSERT_THAT(split_string, testing::ElementsAre("Hello", "my", "name", "is", "stringy!"));
}

TEST(SplitString, TestWideStringSplitCorrectly)
{
   const std::wstring string_to_split(L"Hello my name is stringy!");
   const std::vector<std::wstring> split_string = sage::string::utilities::split(string_to_split, std::wstring(L" "));
   ASSERT_THAT(split_string, testing::ElementsAre(L"Hello", L"my", L"name", L"is", L"stringy!"));
}

TEST(SplitString, TestStringSplitCorrectlyWhenDelimiterNotInString)
{
   const std::string string_to_split("Hello");
   const std::vector<std::string> split_string = sage::string::utilities::split(string_to_split, std::string(" "));
   ASSERT_THAT(split_string, testing::ElementsAre("Hello"));
}

TEST(SplitString, TestStringSplitCorrectlyWhenDelimiterInStringMultipleTimes)
{
   const std::string string_to_split("Hello   ");
   const std::vector<std::string> split_string = sage::string::utilities::split(string_to_split, std::string(" "));
   ASSERT_THAT(split_string, testing::ElementsAre("Hello", "", "", ""));
}

TEST(SplitString, TestStringSplitCorrectlyWhenDelimiterIsMultipleCharacters)
{
   const std::string string_to_split("Hello\nsplit\nstring");
   const std::vector<std::string> split_string = sage::string::utilities::split(string_to_split, std::string("\n"));
   ASSERT_THAT(split_string, testing::ElementsAre("Hello", "split", "string"));
}

TEST(SplitString, TestStringSplitCorrectlyWhenDelimiterIsMultipleCharacters2)
{
   const std::string string_to_split("Hellosplitspring");
   const std::vector<std::string> split_string = sage::string::utilities::split(string_to_split, std::string("sp"));
   ASSERT_THAT(split_string, testing::ElementsAre("Hello", "lit", "ring"));
}

TEST(SplitString, TestStringSplitCorrectlyWideString)
{
   const std::wstring string_to_split(L"Hello my name is stringy!");
   const std::vector<std::wstring> split_string = sage::string::utilities::split(string_to_split, std::wstring(L" "));
   ASSERT_THAT(split_string, testing::ElementsAre(L"Hello", L"my", L"name", L"is", L"stringy!"));
}

TEST(SplitString, TestStringSplitCorrectlyEmptyString)
{
   const std::string string_to_split;
   const std::vector<std::string> split_string = sage::string::utilities::split(string_to_split, std::string(" "));
   ASSERT_THAT(split_string, testing::IsEmpty());
}