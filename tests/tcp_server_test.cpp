#include <cassert>
#include <stdexcept>
#include <string>
#include <utility>
#include <core/tcp_server.hpp>

/// Test that TcpServer can be constructed successfully
void test_construction()
{
    TcpServer server;
    assert(server.is_socket_initialized());
}

/// Test that socket is properly initialized after construction
void test_socket_initialization()
{
    TcpServer server;
    assert(server.is_socket_initialized() == true);
}

/// Test move constructor transfers ownership
void test_move_constructor()
{
    TcpServer server1;
    assert(server1.is_socket_initialized());
    
    TcpServer server2(std::move(server1));
    // After move, server2 should be initialized and server1 should not
    assert(server2.is_socket_initialized());
    assert(!server1.is_socket_initialized());
}

/// Test move assignment transfers ownership and cleans up old socket
void test_move_assignment()
{
    TcpServer server1;
    TcpServer server2;
    
    assert(server1.is_socket_initialized());
    assert(server2.is_socket_initialized());
    
    // Move server1 into server2
    server2 = std::move(server1);
    
    // server2 should still be initialized, server1 should not
    assert(server2.is_socket_initialized());
    assert(!server1.is_socket_initialized());
}

/// Test that start() throws when socket is not initialized
void test_start_without_socket()
{
    TcpServer server1;
    TcpServer server2 = std::move(server1);
    // server1 is now uninitialized
    
    try
    {
        server1.start();
        assert(false && "Expected std::runtime_error");
    }
    catch (const std::runtime_error& e)
    {
        // Expected exception
        assert(std::string(e.what()) == "Socket not initialized");
    }
}

int main()
{
    test_construction();
    test_socket_initialization();
    test_move_constructor();
    test_move_assignment();
    test_start_without_socket();
    
    return 0;
}
