#include <http/result.hpp>

#include <gtest/gtest.h>

#include <string>

TEST(ResultTest, SuccessWithoutMessage)
{
    const Result result = Result::success();

    EXPECT_TRUE(result.is_success());
    EXPECT_FALSE(result.is_failure());
    EXPECT_TRUE(result.error_message().empty());
}

TEST(ResultTest, FailurePreservesMessage)
{
    const std::string message = "Unable to bind socket";
    const Result result = Result::failure(message);

    EXPECT_FALSE(result.is_success());
    EXPECT_TRUE(result.is_failure());
    EXPECT_EQ(result.error_message(), message);
}