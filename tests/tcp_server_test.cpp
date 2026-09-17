#include <http/tcp_server.hpp>

#include <gtest/gtest.h>

TEST(TcpServerTest, StartWithEphemeralPortSucceeds)
{
    TcpServer server;
    const TcpServerConfig config{.port = 0, .max_backlog = 1};

    const Result result = server.start(config);

    EXPECT_TRUE(result.is_success());
    EXPECT_TRUE(result.error_message().empty());
}

TEST(TcpServerTest, StartReturnsFailureWhenCalledTwice)
{
    TcpServer server;
    const TcpServerConfig config{.port = 0, .max_backlog = 1};

    ASSERT_TRUE(server.start(config).is_success());

    const Result result = server.start(config);

    EXPECT_FALSE(result.is_success());
    EXPECT_EQ(result.error_message(), "Socket already initialized");
}
