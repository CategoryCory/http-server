#include <http/tcp_socket.hpp>

#include <cerrno>
#include <cstdint>
#include <stdexcept>
#include <sys/socket.h>
#include <system_error>
#include <utility>

TcpSocket::TcpSocket(UniqueFileDescriptor &&socket_fd, TcpSocketState state)
    : m_socket_fd(std::move(socket_fd)), m_state(state)
{
}

void TcpSocket::initialize_socket()
{
    if (m_addr.sin_family != 0)
    {
        throw std::logic_error("Socket already initialized");
    }

    auto socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to create socket");
    }

    UniqueFileDescriptor temp_fd{socket_fd};

    const int option = 1;
    const int sockopt_result = ::setsockopt(temp_fd.get(), SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (sockopt_result < 0)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to set socket options");
    }

    m_state = TcpSocketState::Initialized;
    m_socket_fd = std::move(temp_fd);
}

void TcpSocket::bind_address(int port)
{
    if (!std::in_range<std::uint16_t>(port))
    {
        throw std::invalid_argument("Port number must be in the range 0-65535");
    }

    if (!is_valid() || m_state != TcpSocketState::Initialized)
    {
        throw std::logic_error("Socket not initialized");
    }

    m_addr = {};

    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_addr.sin_port = htons(port);

    if (::bind(m_socket_fd.get(), reinterpret_cast<const sockaddr *>(&m_addr), sizeof(m_addr)) < 0)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to bind socket");
    }

    m_state = TcpSocketState::Bound;
}

void TcpSocket::listen_for_connections(int max_pending_connections)
{
    if (!is_valid())
    {
        throw std::logic_error("Socket not initialized");
    }

    if (m_state != TcpSocketState::Bound)
    {
        throw std::logic_error("Socket not bound");
    }

    if (::listen(m_socket_fd.get(), max_pending_connections) < 0)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to listen for connections");
    }

    m_state = TcpSocketState::Listening;
}

TcpSocket TcpSocket::accept_connection()
{
    if (!is_valid())
    {
        throw std::logic_error("Socket not initialized");
    }

    if (m_state != TcpSocketState::Listening)
    {
        throw std::logic_error("Socket is not in a listening state");
    }

    sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = ::accept(m_socket_fd.get(), reinterpret_cast<sockaddr *>(&client_addr), &client_addr_len);

    // TODO: Handle EINTR, EAGAIN, and other recoverable errors during accept()
    // TODO: Handle client address information if needed

    if (client_fd < 0)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to accept connection");
    }

    return TcpSocket(UniqueFileDescriptor{client_fd}, TcpSocketState::Connected);
}

bool TcpSocket::is_valid() const noexcept { return m_socket_fd.is_valid(); }

int TcpSocket::get() const noexcept { return m_socket_fd.get(); }
