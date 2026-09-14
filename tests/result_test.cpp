#include <http/result.hpp>

#include <gtest/gtest.h>

#include <string>

TEST(ResultTest, DefaultResultIsFailureWithoutMessage)
{
    const Result result;

    EXPECT_FALSE(result.is_success());
    EXPECT_TRUE(result.message().empty());
}

TEST(ResultTest, SuccessWithoutMessage)
{
    const Result result = Result::success();

    EXPECT_TRUE(result.is_success());
    EXPECT_TRUE(result.message().empty());
}

TEST(ResultTest, SuccessPreservesMessage)
{
    const std::string message = "Request completed";
    const Result result = Result::success(message);

    EXPECT_TRUE(result.is_success());
    EXPECT_EQ(result.message(), message);
}

TEST(ResultTest, FailurePreservesMessage)
{
    const std::string message = "Unable to bind socket";
    const Result result = Result::failure(message);

    EXPECT_FALSE(result.is_success());
    EXPECT_EQ(result.message(), message);
}