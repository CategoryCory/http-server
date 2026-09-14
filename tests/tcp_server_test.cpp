#include <http/tcp_server.hpp>

#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <utility>

TEST(TcpServerTest, ConstructionInitializesSocket)
{
    TcpServer server;
    EXPECT_TRUE(server.is_socket_initialized());
}

TEST(TcpServerTest, SocketIsInitializedAfterConstruction)
{
    TcpServer server;
    EXPECT_TRUE(server.is_socket_initialized());
}

TEST(TcpServerTest, MoveConstructionTransfersSocketOwnership)
{
    TcpServer server1;
    ASSERT_TRUE(server1.is_socket_initialized());

    TcpServer server2(std::move(server1));
    EXPECT_TRUE(server2.is_socket_initialized());

    // NOLINTNEXTLINE(bugprone-use-after-move): verifies the moved-from server is empty.
    EXPECT_FALSE(server1.is_socket_initialized());
}

TEST(TcpServerTest, MoveAssignmentTransfersSocketOwnership)
{
    TcpServer server1;
    TcpServer server2;

    ASSERT_TRUE(server1.is_socket_initialized());
    ASSERT_TRUE(server2.is_socket_initialized());

    server2 = std::move(server1);

    EXPECT_TRUE(server2.is_socket_initialized());

    // NOLINTNEXTLINE(bugprone-use-after-move): verifies the moved-from server is empty.
    EXPECT_FALSE(server1.is_socket_initialized());
}

TEST(TcpServerTest, StartOnMovedFromServerThrows)
{
    TcpServer server1;
    TcpServer server2 = std::move(server1);

    try
    {
        // NOLINTNEXTLINE(bugprone-use-after-move): verifies the moved-from server is empty.
        server1.start();
        FAIL() << "Expected std::runtime_error";
    }
    catch (const std::runtime_error &e)
    {
        EXPECT_EQ(e.what(), std::string("Socket not initialized"));
    }
}
