#pragma once

#include <http_server/tcp/server_error.hpp>
#include <http_server/tcp/tcp_connection.hpp>
#include <http_server/tcp/tcp_listener.hpp>
#include <http_server/tcp/tcp_server_config.hpp>
#include <http_server/tcp/tcp_socket.hpp>

#include <expected>
#include <optional>

namespace http_server::tcp
{

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
    /// @return An engaged std::expected containing the TcpConnection on success; otherwise an unexpected
    /// ServerError.
    [[nodiscard]] std::expected<TcpConnection, ServerError> accept_connection() const;

    /// @brief Checks if the TCP server is currently running.
    /// @return true if the server is running; otherwise false.
    [[nodiscard]] bool is_running() const noexcept { return m_listener.has_value(); }

    /// @brief Stops the TCP server.
    void stop() noexcept;

private:
    std::optional<TcpListener> m_listener;
};

} // namespace http_server::tcp
