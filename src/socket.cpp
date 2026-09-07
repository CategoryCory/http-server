#include <http/socket.hpp>

#include <utility>

Socket::Socket(int fd) : m_socket_fd(fd) {}

Socket::Socket(Socket &&other) noexcept
    : m_socket_fd(std::exchange(other.m_socket_fd, UniqueFileDescriptor{})) {}

Socket& Socket::operator=(Socket &&other) noexcept
{
    if (this != &other)
    {
        m_socket_fd = std::exchange(other.m_socket_fd, UniqueFileDescriptor{});
    }

    return *this;
}

Socket::~Socket()
{
    if (m_socket_fd.is_valid())
    {
        m_socket_fd.reset();
    }
}

bool Socket::is_valid() const noexcept
{
    return m_socket_fd.is_valid();
}

int Socket::get() const noexcept
{
    return m_socket_fd.get();
}
