#include <netinet/in.h>
#include <stdexcept>
#include <unistd.h>
#include <utility>
#include <core/tcp_server.hpp>

TcpServer::TcpServer()
{
    m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket_fd < 0)
    {
        throw std::runtime_error("Failed to create socket");
    }

    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    m_server_addr.sin_port = htons(DEFAULT_PORT);
}

TcpServer::TcpServer(TcpServer&& other) noexcept
    : m_socket_fd(std::exchange(other.m_socket_fd, -1)),
      m_server_addr(std::exchange(other.m_server_addr, sockaddr_in{})) { }

TcpServer& TcpServer::operator=(TcpServer&& other) noexcept
{
    if (this != &other)
    {
        if (m_socket_fd != -1)
        {
            close(m_socket_fd);
        }

        m_socket_fd = std::exchange(other.m_socket_fd, -1);
        m_server_addr = std::exchange(other.m_server_addr, sockaddr_in{});
    }
    return *this;
}

TcpServer::~TcpServer()
{
    if (m_socket_fd != -1)
    {
        close(m_socket_fd);
    }
}

bool TcpServer::is_socket_initialized() const
{
    return m_socket_fd != -1;
}

void TcpServer::start()
{
    if (m_socket_fd == -1)
    {
        throw std::runtime_error("Socket not initialized");
    }

    if (bind(m_socket_fd, reinterpret_cast<sockaddr*>(&m_server_addr), sizeof(m_server_addr)) < 0)
    {
        throw std::runtime_error("Failed to bind socket");
    }

    if (listen(m_socket_fd, 5) < 0)
    {
        throw std::runtime_error("Failed to listen on socket");
    }
}
