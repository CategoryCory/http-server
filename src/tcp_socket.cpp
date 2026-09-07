#include <http/tcp_socket.hpp>

#include <cstdint>
#include <cerrno>
#include <stdexcept>
#include <sys/socket.h>
#include <system_error>
#include <utility>

void TcpSocket::initialize_socket()
{
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

    m_socket_fd = std::move(temp_fd);
}

void TcpSocket::bind_address(int port)
{
    if (!std::in_range<std::uint16_t>(port))
    {
        throw std::invalid_argument("Port number must be in the range 0-65535");
    }

    if (!m_socket_fd.is_valid())
    {
        throw std::logic_error("Socket not initialized");
    }

    m_server_addr = {};

    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_server_addr.sin_port = htons(port);

    if (::bind(m_socket_fd.get(), reinterpret_cast<const sockaddr*>(&m_server_addr), sizeof(m_server_addr)) < 0)
    {
        throw std::system_error(errno, std::generic_category(), "Failed to bind socket");
    }
}

bool TcpSocket::is_valid() const noexcept
{
    return m_socket_fd.is_valid();
}

int TcpSocket::get() const noexcept
{
    return m_socket_fd.get();
}
