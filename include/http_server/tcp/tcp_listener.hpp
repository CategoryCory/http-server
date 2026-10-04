#pragma once

#include <http_server/core/unique_file_descriptor.hpp>
#include <http_server/tcp/socket_error.hpp>

#include <cstdint>
#include <expected>

namespace http_server::tcp
{

/// @brief TCP listener for incoming connections
class TcpListener
{
public:
    /// @brief Creates an instance of TcpListener
    /// @param port Port to listen on
    /// @param max_backlog Max pending connections
    /// @return `std::expected<TcpListener, SocketError>` containing the listener or error
    [[nodiscard]] static std::expected<TcpListener, SocketError> create(std::uint16_t port, int max_backlog);

private:
    explicit TcpListener(core::UniqueFileDescriptor &&fd) noexcept;
    core::UniqueFileDescriptor m_fd;
};
} // namespace http_server::tcp
