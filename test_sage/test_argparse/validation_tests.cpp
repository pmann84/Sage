#include <sage/argparse/argparse.hpp>

#include "gtest/gtest.h"

TEST(ValidationTests, TestDoubleDashArgumentIsOptional)
{
    ASSERT_TRUE(sage::argparse::validate::is_optional("--foo"));
}

TEST(ValidationTests, TestSingleDashArgumentIsOptional)
{
    ASSERT_TRUE(sage::argparse::validate::is_optional("-foo"));
}

TEST(ValidationTests, TestPositionalArgumentIsNotOptional)
{
    ASSERT_FALSE(sage::argparse::validate::is_optional("foo"));
}