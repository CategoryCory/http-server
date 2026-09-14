#include <http/tcp_socket.hpp>

#include <arpa/inet.h>
#include <gtest/gtest.h>

#include <stdexcept>
#include <sys/socket.h>

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

TEST(TcpSocketTest, BindingRequiresInitializedSocket)
{
    TcpSocket socket;

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