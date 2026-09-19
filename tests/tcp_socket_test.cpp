#include <http/tcp_socket.hpp>
#include <http/unique_file_descriptor.hpp>

#include <arpa/inet.h>
#include <gtest/gtest.h>

#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

TEST(TcpSocketTest, DefaultConstructionIsInvalid)
{
    const TcpSocket socket;

    EXPECT_FALSE(socket.is_valid());
    EXPECT_EQ(socket.get(), -1);
}

TEST(TcpSocketTest, InitializationCreatesSocket)
{
    TcpSocket socket;

    ASSERT_TRUE(socket.initialize_socket());

    EXPECT_TRUE(socket.is_valid());
    EXPECT_GE(socket.get(), 0);
}

TEST(TcpSocketTest, InitializationEnablesAddressReuse)
{
    TcpSocket socket;
    ASSERT_TRUE(socket.initialize_socket());

    int option{};
    socklen_t option_length = sizeof(option);
    ASSERT_EQ(getsockopt(socket.get(), SOL_SOCKET, SO_REUSEADDR, &option, &option_length), 0);
    EXPECT_NE(option, 0);
}

TEST(TcpSocketTest, InitializationCannotBeRepeated)
{
    TcpSocket socket;
    ASSERT_TRUE(socket.initialize_socket());

    const auto result = socket.initialize_socket();

    ASSERT_FALSE(result);
    EXPECT_EQ(result.error().code, SocketErrorCode::invalid_state);
}

TEST(TcpSocketTest, BindingRequiresInitializedSocket)
{
    TcpSocket socket;

    const auto result = socket.bind_address(0);

    ASSERT_FALSE(result);
    EXPECT_EQ(result.error().code, SocketErrorCode::invalid_state);
}

TEST(TcpSocketTest, BindingCannotBeRepeated)
{
    TcpSocket socket;
    ASSERT_TRUE(socket.initialize_socket());
    ASSERT_TRUE(socket.bind_address(0));

    const auto result = socket.bind_address(0);

    ASSERT_FALSE(result);
    EXPECT_EQ(result.error().code, SocketErrorCode::invalid_state);
}

class TcpSocketInvalidPortTest : public testing::TestWithParam<int>
{
};

TEST_P(TcpSocketInvalidPortTest, BindingRejectsPortOutsideValidRange)
{
    TcpSocket socket;

    const auto result = socket.bind_address(GetParam());

    ASSERT_FALSE(result);
    EXPECT_EQ(result.error().code, SocketErrorCode::invalid_port);
}

INSTANTIATE_TEST_SUITE_P(OutOfRange, TcpSocketInvalidPortTest, testing::Values(-1, 65536));

TEST(TcpSocketTest, BindingToEphemeralPortAssignsPort)
{
    TcpSocket socket;
    ASSERT_TRUE(socket.initialize_socket());
    ASSERT_TRUE(socket.bind_address(0));

    sockaddr_in address{};
    socklen_t address_length = sizeof(address);
    ASSERT_EQ(getsockname(socket.get(), reinterpret_cast<sockaddr *>(&address), &address_length), 0);
    EXPECT_EQ(address.sin_family, AF_INET);
    EXPECT_NE(ntohs(address.sin_port), 0);
}

TEST(TcpSocketTest, ListeningRequiresBoundSocket)
{
    TcpSocket uninitialized_socket;
    const auto uninitialized_result = uninitialized_socket.listen_for_connections(1);
    ASSERT_FALSE(uninitialized_result);
    EXPECT_EQ(uninitialized_result.error().code, SocketErrorCode::invalid_state);

    TcpSocket initialized_socket;
    ASSERT_TRUE(initialized_socket.initialize_socket());
    const auto initialized_result = initialized_socket.listen_for_connections(1);
    ASSERT_FALSE(initialized_result);
    EXPECT_EQ(initialized_result.error().code, SocketErrorCode::invalid_state);
}

TEST(TcpSocketTest, ListeningCannotBeRepeated)
{
    TcpSocket socket;
    ASSERT_TRUE(socket.initialize_socket());
    ASSERT_TRUE(socket.bind_address(0));
    ASSERT_TRUE(socket.listen_for_connections(1));

    const auto result = socket.listen_for_connections(1);

    ASSERT_FALSE(result);
    EXPECT_EQ(result.error().code, SocketErrorCode::invalid_state);
}

TEST(TcpSocketTest, AcceptingRequiresListeningSocket)
{
    TcpSocket uninitialized_socket;
    const auto uninitialized_result = uninitialized_socket.accept_connection();
    ASSERT_FALSE(uninitialized_result);
    EXPECT_EQ(uninitialized_result.error().code, SocketErrorCode::invalid_state);

    TcpSocket initialized_socket;
    ASSERT_TRUE(initialized_socket.initialize_socket());
    const auto initialized_result = initialized_socket.accept_connection();
    ASSERT_FALSE(initialized_result);
    EXPECT_EQ(initialized_result.error().code, SocketErrorCode::invalid_state);

    TcpSocket bound_socket;
    ASSERT_TRUE(bound_socket.initialize_socket());
    ASSERT_TRUE(bound_socket.bind_address(0));
    const auto bound_result = bound_socket.accept_connection();
    ASSERT_FALSE(bound_result);
    EXPECT_EQ(bound_result.error().code, SocketErrorCode::invalid_state);
}

TEST(TcpSocketTest, AcceptingConnectionReturnsConnectedSocket)
{
    TcpSocket listener;
    ASSERT_TRUE(listener.initialize_socket());
    ASSERT_TRUE(listener.bind_address(0));
    ASSERT_TRUE(listener.listen_for_connections(1));

    sockaddr_in listener_address{};
    socklen_t listener_address_length = sizeof(listener_address);
    ASSERT_EQ(getsockname(listener.get(), reinterpret_cast<sockaddr *>(&listener_address), &listener_address_length),
              0);

    UniqueFileDescriptor client_socket(::socket(AF_INET, SOCK_STREAM, 0));
    ASSERT_TRUE(client_socket.is_valid());

    listener_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    ASSERT_EQ(
        ::connect(client_socket.get(), reinterpret_cast<const sockaddr *>(&listener_address), sizeof(listener_address)),
        0);

    auto connection_result = listener.accept_connection();
    ASSERT_TRUE(connection_result);
    TcpSocket connection = std::move(*connection_result);

    EXPECT_TRUE(connection.is_valid());
    EXPECT_GE(connection.get(), 0);
    const auto result = connection.accept_connection();
    ASSERT_FALSE(result);
    EXPECT_EQ(result.error().code, SocketErrorCode::invalid_state);
}