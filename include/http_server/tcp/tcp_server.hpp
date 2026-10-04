#pragma once

#include <http_server/tcp/server_error.hpp>
#include <http_server/tcp/tcp_connection.hpp>
#include <http_server/tcp/tcp_listener.hpp>
#include <http_server/tcp/tcp_server_config.hpp>

#include <expected>
#include <optional>

namespace http_server::tcp
{

/// @brief A TCP server that listens for incoming client connections.
///
/// Manages a TCP socket bound to a local port and accepts incoming client connections. It is designed to be
/// used as a building block for higher-level network protocols such as HTTP servers.
class TcpServer
{
public:
    /// @brief Creates a server that is not running.
    TcpServer() = default;

    /// @brief Starts the TCP server and begins listening for incoming connections.
    /// @param server_config The configuration for the TCP server.
    /// @return An engaged std::expected on success; otherwise an unexpected ServerError with code
    ///         ServerErrorCode::already_running or ServerErrorCode::socket_failure.
    [[nodiscard]] std::expected<void, ServerError> start(const TcpServerConfig &server_config);

    /// @brief Accepts an incoming client connection.
    ///
    /// Blocks until a client connects or an error occurs.
    /// @return A std::expected containing the TcpConnection on success; otherwise an unexpected ServerError with
    ///         code ServerErrorCode::not_running or ServerErrorCode::socket_failure.
    [[nodiscard]] std::expected<TcpConnection, ServerError> accept_connection() const;

    /// @brief Checks whether the TCP server is currently running.
    /// @return true when the server is running; otherwise false.
    [[nodiscard]] bool is_running() const noexcept { return m_listener.has_value(); }

    /// @brief Stops the TCP server and closes its listening socket.
    ///
    /// Has no effect if the server is not running.
    void stop() noexcept;

private:
    std::optional<TcpListener> m_listener;
};

} // namespace http_server::tcp
