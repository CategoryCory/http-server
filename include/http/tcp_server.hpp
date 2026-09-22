#pragma once

#include <config/tcp_server_config.hpp>
#include <http/server_error.hpp>
#include <http/tcp_socket.hpp>

#include <cstdint>
#include <expected>

/// @brief Represents the state of the TCP server.
enum class TcpServerState : std::uint8_t
{
    /// @brief The server is stopped and not currently running.
    Stopped,

    /// @brief The server is currently running and accepting connections.
    Running
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
    /// @return An engaged std::expected on success; otherwise an unexpected ServerError.
    [[nodiscard]] std::expected<void, ServerError> start(const TcpServerConfig &server_config);

    /// @brief Accepts an incoming client connection.
    /// @return An engaged std::expected containing the accepted TcpSocket on success; otherwise an unexpected
    /// ServerError.
    [[nodiscard]] std::expected<TcpSocket, ServerError> accept_connection() const;

    /// @brief Checks if the TCP server is currently running.
    /// @return true if the server is running; otherwise false.
    [[nodiscard]] bool is_running() const noexcept { return m_state == TcpServerState::Running; }

    /// @brief Stops the TCP server.
    void stop() noexcept;

private:
    /// @brief TCP socket for the server
    TcpSocket m_socket{};
    TcpServerState m_state{TcpServerState::Stopped};
};
