#pragma once

#include <http/result.hpp>
#include <http/tcp_socket.hpp>

#include <cstdint>

/// @brief Default port number for the TCP server
constexpr std::uint16_t DEFAULT_PORT = 8080;

/// @brief Maximum number of pending connections the server can have in its listen queue
/// @note This value is used when calling the listen_for_connections method on the server socket
constexpr int MAX_BACKLOG = 5;

/// @brief A TCP server that listens for incoming client connections
/// 
/// This class manages a TCP socket bound to a local address and port,
/// allowing it to accept incoming client connections. It is designed
/// to be used as a building block for higher-level network protocols
/// such as HTTP servers.
class TcpServer
{
public:
    /// @brief Default constructor for the TCP server
    TcpServer() = default;

    /// @brief Starts the TCP server and begins listening for incoming connections
    /// @param port The port number on which the server should listen (default is 8080)
    /// @param max_backlog The maximum number of pending connections in the listen queue (default is 5)
    Result start(std::uint16_t port = DEFAULT_PORT, int max_backlog = MAX_BACKLOG);

    /// @brief Stops the TCP server
    void stop();
private:
    /// @brief TCP socket for the server
    TcpSocket m_socket{};
};
