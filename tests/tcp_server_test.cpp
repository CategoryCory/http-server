#include <http/tcp_server.hpp>

#include <gtest/gtest.h>

TEST(TcpServerTest, StartWithEphemeralPortSucceeds)
{
    TcpServer server;
    const TcpServerConfig config{.port = 0, .max_backlog = 1};

    const auto result = server.start(config);

    EXPECT_TRUE(result);
}

TEST(TcpServerTest, StartReturnsFailureWhenCalledTwice)
{
    TcpServer server;
    const TcpServerConfig config{.port = 0, .max_backlog = 1};

    ASSERT_TRUE(server.start(config));

    const auto result = server.start(config);

    ASSERT_FALSE(result);
    EXPECT_EQ(result.error().code, ServerErrorCode::already_running);
}
