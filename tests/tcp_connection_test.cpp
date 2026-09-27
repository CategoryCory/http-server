#include <http/tcp_connection.hpp>
#include <http/unique_file_descriptor.hpp>

#include <arpa/inet.h>
#include <gtest/gtest.h>

TEST(TcpConnectionTest, GetClientIpAddressReturnsCorrectIp)
{
    sockaddr_in client_address{};
    client_address.sin_family = AF_INET;
    ASSERT_EQ(::inet_pton(AF_INET, "203.0.113.42", &client_address.sin_addr), 1);

    TcpConnection connection(UniqueFileDescriptor{}, client_address);

    const auto result = connection.get_client_ip_address();

    ASSERT_TRUE(result);
    ASSERT_EQ(*result, "203.0.113.42");
}
