#include <http/tcp_socket.hpp>

#include <cerrno>
#include <sys/socket.h>
#include <utility>

TcpSocket::TcpSocket(UniqueFileDescriptor &&socket_fd, TcpSocketState state)
    : m_socket_fd(std::move(socket_fd)), m_state(state)
{
}

std::expected<void, SocketError> TcpSocket::initialize_socket()
{
    if (m_state != TcpSocketState::Uninitialized)
    {
        return std::unexpected(SocketError{.code = SocketErrorCode::invalid_state,
                                           .diagnostic = "Socket already initialized"});
    }

    const auto socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0)
    {
        return std::unexpected(SocketError{.code = SocketErrorCode::system_error,
                                           .error_code = {errno, std::generic_category()},
                                           .diagnostic = "Failed to create socket"});
    }

    UniqueFileDescriptor temp_fd{socket_fd};

    constexpr int option = 1;
    const int sockopt_result = ::setsockopt(temp_fd.get(), SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (sockopt_result < 0)
    {
        return std::unexpected(SocketError{.code = SocketErrorCode::system_error,
                                           .error_code = {errno, std::generic_category()},
                                           .diagnostic = "Failed to set socket options"});
    }

    m_socket_fd = std::move(temp_fd);
    m_state = TcpSocketState::Initialized;
    return {};
}

std::expected<void, SocketError> TcpSocket::bind_address(int port)
{
    if (!std::in_range<std::uint16_t>(port))
    {
        return std::unexpected(SocketError{.code = SocketErrorCode::invalid_port,
                                           .diagnostic = "Port number must be in the range 0-65535"});
    }

    if (const auto state_result = require_socket_state(TcpSocketState::Initialized); !state_result)
    {
        return std::unexpected(state_result.error());
    }

    m_addr = {};

    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_addr.sin_port = htons(port);

    if (::bind(m_socket_fd.get(), reinterpret_cast<const sockaddr *>(&m_addr), sizeof(m_addr)) < 0)
    {
        return std::unexpected(SocketError{.code = SocketErrorCode::system_error,
                                           .error_code = {errno, std::generic_category()},
                                           .diagnostic = "Failed to bind socket"});
    }

    m_state = TcpSocketState::Bound;
    return {};
}

std::expected<void, SocketError> TcpSocket::listen_for_connections(int max_pending_connections)
{
    if (const auto state_result = require_socket_state(TcpSocketState::Bound); !state_result)
    {
        return std::unexpected(state_result.error());
    }

    if (::listen(m_socket_fd.get(), max_pending_connections) < 0)
    {
        return std::unexpected(SocketError{.code = SocketErrorCode::system_error,
                                           .error_code = {errno, std::generic_category()},
                                           .diagnostic = "Failed to listen for connections"});
    }

    m_state = TcpSocketState::Listening;
    return {};
}

std::expected<TcpSocket, SocketError> TcpSocket::accept_connection() const
{
    if (const auto state_result = require_socket_state(TcpSocketState::Listening); !state_result)
    {
        return std::unexpected(state_result.error());
    }

    sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);
    const int client_fd = ::accept(m_socket_fd.get(), reinterpret_cast<sockaddr *>(&client_addr), &client_addr_len);

    // TODO: Handle EINTR, EAGAIN, and other recoverable errors during accept()
    // TODO: Handle client address information if needed

    if (client_fd < 0)
    {
        return std::unexpected(SocketError{.code = SocketErrorCode::system_error,
                                           .error_code = {errno, std::generic_category()},
                                           .diagnostic = "Failed to accept connection"});
    }

    return TcpSocket(UniqueFileDescriptor{client_fd}, TcpSocketState::Connected);
}

bool TcpSocket::is_valid() const noexcept { return m_socket_fd.is_valid(); }

int TcpSocket::get() const noexcept { return m_socket_fd.get(); }

std::expected<void, SocketError> TcpSocket::require_socket_state(TcpSocketState required_state) const
{
    if (!is_valid() || m_state != required_state)
    {
        return std::unexpected(
            SocketError{.code = SocketErrorCode::invalid_state, .diagnostic = "Socket is not in the required state"});
    }

    return {};
}
