#pragma once

#include <http_server/core/unique_file_descriptor.hpp>
#include <http_server/tcp/socket_error.hpp>
#include <http_server/tcp/tcp_connection.hpp>

#include <cstdint>
#include <expected>

namespace http_server::tcp
{

/// @brief Listens for incoming TCP connections.
///
/// Owns a listening socket that is closed when the listener is destroyed.
class TcpListener
{
public:
    /// @brief Creates a listener bound to a port on all local IPv4 interfaces.
    /// @param port The port to listen on.
    /// @param max_backlog The maximum number of pending connections in the listen queue.
    /// @return A std::expected containing the TcpListener on success; otherwise an unexpected SocketError.
    [[nodiscard]] static std::expected<TcpListener, SocketError> create(std::uint16_t port, int max_backlog);

    /// @brief Accepts an incoming TCP connection.
    ///
    /// Blocks until a client connects or an error occurs.
    /// @return A std::expected containing the TcpConnection on success; otherwise an unexpected SocketError.
    [[nodiscard]] std::expected<TcpConnection, SocketError> accept_connection() const;

private:
    explicit TcpListener(core::UniqueFileDescriptor &&fd) noexcept;
    core::UniqueFileDescriptor m_fd;
};
} // namespace http_server::tcp
