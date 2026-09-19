#pragma once

#include <config/tcp_server_config.hpp>
#include <expected>
#include <http/server_error.hpp>
#include <http/tcp_socket.hpp>

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
    /// @return An engaged std::expected on success; otherwise an unexpected ServerError.
    [[nodiscard]] std::expected<void, ServerError> start(const TcpServerConfig &server_config);

    /// @brief Stops the TCP server.
    /// @throws std::runtime_error because this operation is not yet implemented.
    void stop();

private:
    /// @brief TCP socket for the server
    TcpSocket m_socket{};
};
