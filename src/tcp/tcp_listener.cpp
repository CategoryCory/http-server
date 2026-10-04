#include <http_server/tcp/tcp_listener.hpp>

#include <http_server/core/unique_file_descriptor.hpp>
#include <http_server/tcp/socket_error.hpp>

#include "platform.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <cstdint>
#include <expected>
#include <netinet/in.h>
#include <sys/socket.h>
#include <system_error>
#include <utility>

namespace http_server::tcp
{
namespace
{
[[nodiscard]] std::expected<core::UniqueFileDescriptor, SocketError> initialize_socket()
{
    const auto socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0)
    {
        return std::unexpected(SocketError{
            .code = SocketErrorCode::system_error,
            .error_code = {errno, std::generic_category()},
            .diagnostic = "Failed to create socket",
        });
    }

    core::UniqueFileDescriptor fd{socket_fd};

    constexpr int option = 1;
    const int sockopt_result = ::setsockopt(fd.get(), SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (sockopt_result < 0)
    {
        return std::unexpected(SocketError{
            .code = SocketErrorCode::system_error,
            .error_code = {errno, std::generic_category()},
            .diagnostic = "Failed to set socket options",
        });
    }

    return fd;
}

[[nodiscard]] std::expected<void, SocketError> bind_address(const core::UniqueFileDescriptor &fd, std::uint16_t port)
{
    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (::bind(fd.get(), reinterpret_cast<const sockaddr *>(&addr), sizeof(addr)) < 0)
    {
        return std::unexpected(SocketError{
            .code = SocketErrorCode::system_error,
            .error_code = {errno, std::generic_category()},
            .diagnostic = "Failed to bind socket",
        });
    }

    return {};
}

[[nodiscard]] std::expected<void, SocketError> listen_for_connections(const core::UniqueFileDescriptor &fd,
                                                                      int max_backlog)
{
    if (::listen(fd.get(), max_backlog) < 0)
    {
        return std::unexpected(SocketError{
            .code = SocketErrorCode::system_error,
            .error_code = {errno, std::generic_category()},
            .diagnostic = "Failed to listen for connections",
        });
    }

    return {};
}
} // namespace

TcpListener::TcpListener(core::UniqueFileDescriptor &&fd) noexcept
    : m_fd(std::move(fd))
{
}

std::expected<TcpListener, SocketError> TcpListener::create(std::uint16_t port, int max_backlog)
{
    return initialize_socket().and_then(
        [port, max_backlog](core::UniqueFileDescriptor fd)
        {
            return bind_address(fd, port)
                .and_then([&fd, max_backlog] { return listen_for_connections(fd, max_backlog); })
                .transform([&fd]() noexcept { return TcpListener{std::move(fd)}; });
        });
}

std::expected<TcpConnection, SocketError> TcpListener::accept_connection() const
{
    sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);
    const int fd = ::accept(m_fd.get(), reinterpret_cast<sockaddr *>(&client_addr), &client_addr_len);

    // TODO: Handle EINTR, EAGAIN, and other recoverable errors during accept()

    if (fd < 0)
    {
        return std::unexpected(SocketError{
            .code = SocketErrorCode::system_error,
            .error_code = {errno, std::generic_category()},
            .diagnostic = "Failed to accept connection",
        });
    }

    core::UniqueFileDescriptor client_fd{fd};

    if (bool configure_result = configure_connected_socket(client_fd.get()); !configure_result)
    {
        return std::unexpected(SocketError{.code = SocketErrorCode::configuration_error,
                                           .diagnostic = "Error occurred when configuring socket"});
    }

    return TcpConnection(std::move(client_fd), client_addr);
}

void TcpListener::close() noexcept
{
    if (m_fd.is_valid())
    {
        m_fd.reset();
    }
}
} // namespace http_server::tcp
