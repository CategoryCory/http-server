#pragma once

#include <http/result.hpp>
#include <http/tcp_socket.hpp>

/// @brief Default port number for the TCP server
constexpr std::uint16_t DEFAULT_PORT = 8080;

/// @brief Maximum number of pending connections the server can have in its listen queue
/// @note This value is used when calling the listen_for_connections method on the server socket
constexpr int MAX_BACKLOG = 5;

/// @brief Configuration structure for the TCP server
struct TcpServerConfig {
    /// @brief Port number the server will listen on
    std::uint16_t port{};

    /// @brief Maximum number of pending connections the server can have in its listen queue
    int max_backlog{};
};

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
    /// @param server_config Configuration for the TCP server
    Result start(const TcpServerConfig& server_config);

    /// @brief Stops the TCP server
    void stop();
private:
    /// @brief TCP socket for the server
    TcpSocket m_socket{};
};
