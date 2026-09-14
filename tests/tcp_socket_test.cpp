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

    socket.initialize_socket();

    EXPECT_TRUE(socket.is_valid());
    EXPECT_GE(socket.get(), 0);
}

TEST(TcpSocketTest, InitializationEnablesAddressReuse)
{
    TcpSocket socket;
    socket.initialize_socket();

    int option{};
    socklen_t option_length = sizeof(option);
    ASSERT_EQ(getsockopt(socket.get(), SOL_SOCKET, SO_REUSEADDR, &option, &option_length), 0);
    EXPECT_NE(option, 0);
}

TEST(TcpSocketTest, InitializationCannotBeRepeated)
{
    TcpSocket socket;
    socket.initialize_socket();

    EXPECT_THROW(socket.initialize_socket(), std::logic_error);
}

TEST(TcpSocketTest, BindingRequiresInitializedSocket)
{
    TcpSocket socket;

    EXPECT_THROW(socket.bind_address(0), std::logic_error);
}

TEST(TcpSocketTest, BindingCannotBeRepeated)
{
    TcpSocket socket;
    socket.initialize_socket();
    socket.bind_address(0);

    EXPECT_THROW(socket.bind_address(0), std::logic_error);
}

class TcpSocketInvalidPortTest : public testing::TestWithParam<int>
{
};

TEST_P(TcpSocketInvalidPortTest, BindingRejectsPortOutsideValidRange)
{
    TcpSocket socket;

    EXPECT_THROW(socket.bind_address(GetParam()), std::invalid_argument);
}

INSTANTIATE_TEST_SUITE_P(OutOfRange, TcpSocketInvalidPortTest, testing::Values(-1, 65536));

TEST(TcpSocketTest, BindingToEphemeralPortAssignsPort)
{
    TcpSocket socket;
    socket.initialize_socket();
    socket.bind_address(0);

    sockaddr_in address{};
    socklen_t address_length = sizeof(address);
    ASSERT_EQ(getsockname(socket.get(), reinterpret_cast<sockaddr *>(&address), &address_length), 0);
    EXPECT_EQ(address.sin_family, AF_INET);
    EXPECT_NE(ntohs(address.sin_port), 0);
}

TEST(TcpSocketTest, ListeningRequiresBoundSocket)
{
    TcpSocket uninitialized_socket;
    EXPECT_THROW(uninitialized_socket.listen_for_connections(1), std::logic_error);

    TcpSocket initialized_socket;
    initialized_socket.initialize_socket();
    EXPECT_THROW(initialized_socket.listen_for_connections(1), std::logic_error);
}

TEST(TcpSocketTest, ListeningCannotBeRepeated)
{
    TcpSocket socket;
    socket.initialize_socket();
    socket.bind_address(0);
    socket.listen_for_connections(1);

    EXPECT_THROW(socket.listen_for_connections(1), std::logic_error);
}

TEST(TcpSocketTest, AcceptingRequiresListeningSocket)
{
    TcpSocket uninitialized_socket;
    EXPECT_THROW(uninitialized_socket.accept_connection(), std::logic_error);

    TcpSocket initialized_socket;
    initialized_socket.initialize_socket();
    EXPECT_THROW(initialized_socket.accept_connection(), std::logic_error);

    TcpSocket bound_socket;
    bound_socket.initialize_socket();
    bound_socket.bind_address(0);
    EXPECT_THROW(bound_socket.accept_connection(), std::logic_error);
}

TEST(TcpSocketTest, AcceptingConnectionReturnsConnectedSocket)
{
    TcpSocket listener;
    listener.initialize_socket();
    listener.bind_address(0);
    listener.listen_for_connections(1);

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

    TcpSocket connection = listener.accept_connection();

    EXPECT_TRUE(connection.is_valid());
    EXPECT_GE(connection.get(), 0);
    EXPECT_THROW(connection.accept_connection(), std::logic_error);
}