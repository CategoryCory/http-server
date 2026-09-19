#include <gtest/gtest.h>
#include <http/tcp_server.hpp>

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
    EXPECT_EQ(result.error().code, ServerErrorCode::socket_failure);
    EXPECT_EQ(result.error().socket_error.code, SocketErrorCode::invalid_state);
    EXPECT_EQ(result.error().socket_error.diagnostic, "Socket already initialized");
}
