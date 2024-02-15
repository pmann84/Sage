#include <sage/string/utilities.hpp>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

TEST(ReplaceString, TestStringSingleReplacement)
{
   const std::string str = "Hello, my name is Paul.";
   const std::string str_to_replace = "Paul";
   const std::string replacement = "Peter";
   const std::string final_str = sage::string::utilities::replace_all(str, str_to_replace, replacement);
   ASSERT_EQ(final_str, "Hello, my name is Peter.");
}

TEST(ReplaceString, TestStringMultipleReplacement)
{
   const std::string str = "Hello Paul, my name is Paul.";
   const std::string str_to_replace = "Paul";
   const std::string replacement = "Peter";
   const std::string final_str = sage::string::utilities::replace_all(str, str_to_replace, replacement);
   ASSERT_EQ(final_str, "Hello Peter, my name is Peter.");
}

TEST(ReplaceString, TestStringReplacementWithQuotes)
{
   const std::string str = R"("Hello, my name is Paul.")";
   const std::string str_to_replace = R"(")";
   const std::string replacement = R"(')";
   const std::string final_str = sage::string::utilities::replace_all(str, str_to_replace, replacement);
   ASSERT_EQ(final_str, R"('Hello, my name is Paul.')");
}