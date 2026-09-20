#include <http/tcp_server.hpp>
#include <http/unique_file_descriptor.hpp>

#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <sys/socket.h>
#include <unistd.h>

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

TEST(TcpServerTest, AcceptingWhenStoppedReturnsNotRunning)
{
    TcpServer server;

    const auto result = server.accept_connection();

    ASSERT_FALSE(result);
    EXPECT_EQ(result.error().code, ServerErrorCode::not_running);
}

TEST(TcpServerTest, AcceptingConnectionReturnsConnectedSocket)
{
    UniqueFileDescriptor reservation(::socket(AF_INET, SOCK_STREAM, 0));
    ASSERT_TRUE(reservation.is_valid());

    sockaddr_in listener_address{};
    listener_address.sin_family = AF_INET;
    listener_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    ASSERT_EQ(::bind(reservation.get(), reinterpret_cast<const sockaddr *>(&listener_address), sizeof(listener_address)),
              0);

    socklen_t listener_address_length = sizeof(listener_address);
    ASSERT_EQ(getsockname(reservation.get(), reinterpret_cast<sockaddr *>(&listener_address), &listener_address_length),
              0);
    reservation.reset();

    TcpServer server;
    ASSERT_TRUE(server.start(TcpServerConfig{.port = ntohs(listener_address.sin_port), .max_backlog = 1}));

    UniqueFileDescriptor client_socket(::socket(AF_INET, SOCK_STREAM, 0));
    ASSERT_TRUE(client_socket.is_valid());
    ASSERT_EQ(
        ::connect(client_socket.get(), reinterpret_cast<const sockaddr *>(&listener_address), sizeof(listener_address)), 0);

    const auto result = server.accept_connection();

    ASSERT_TRUE(result);
    EXPECT_TRUE(result->is_valid());
    EXPECT_GE(result->get(), 0);
}

TEST(TcpServerTest, StopAllowsServerToRestart)
{
    TcpServer server;
    const TcpServerConfig config{.port = 0, .max_backlog = 1};
    ASSERT_TRUE(server.start(config));

    server.stop();

    EXPECT_FALSE(server.is_running());
    EXPECT_TRUE(server.start(config));
    EXPECT_TRUE(server.is_running());
}
