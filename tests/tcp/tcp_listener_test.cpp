#include <http_server/tcp/socket_error.hpp>
#include <http_server/tcp/tcp_listener.hpp>

#include <cstdint>
#include <gtest/gtest.h>

namespace http_server::tcp
{

TEST(TcpListenerTest, AvailablePortReturnsListener)
{
    // Arrange
    const std::uint16_t port{1234};
    const int max_backlog{5};

    // Act
    const auto listener = TcpListener::create(port, max_backlog);

    // Assert
    ASSERT_TRUE(listener);
}

TEST(TcpListenerTest, InUsePortReturnsError)
{
    // Arrange
    const std::uint16_t port{1234};
    const int max_backlog{5};

    const auto first = TcpListener::create(port, max_backlog);
    ASSERT_TRUE(first);

    // Act
    const auto second = TcpListener::create(port, max_backlog);

    // Assert
    ASSERT_FALSE(second);
    ASSERT_EQ(second.error().code, SocketErrorCode::system_error);
    ASSERT_EQ(second.error().diagnostic, "Failed to bind socket");
}

TEST(TcpListenerTest, DestroyingListenerFreesSocket)
{
    // Arrange
    const std::uint16_t port{1234};
    const int max_backlog{5};

    {
        const auto first = TcpListener::create(port, max_backlog);
        ASSERT_TRUE(first);
    }

    // Act
    const auto second = TcpListener::create(port, max_backlog);

    // Assert
    ASSERT_TRUE(second);
}
}
