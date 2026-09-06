#include <http/tcp_server.hpp>

#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>

TcpServer::TcpServer()
{
    m_socket_fd.reset(socket(AF_INET, SOCK_STREAM, 0));
    if (m_socket_fd.get() < 0)
    {
        throw std::runtime_error("Failed to create socket");
    }

    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    m_server_addr.sin_port = htons(DEFAULT_PORT);
}

TcpServer::TcpServer(TcpServer&& other) noexcept
    : m_socket_fd(std::exchange(other.m_socket_fd, UniqueFileDescriptor{})),
      m_server_addr(other.m_server_addr) { }

TcpServer& TcpServer::operator=(TcpServer&& other) noexcept
{
    if (this != &other)
    {
        m_socket_fd = std::exchange(other.m_socket_fd, UniqueFileDescriptor{});
        m_server_addr = other.m_server_addr;
    }

    return *this;
}

TcpServer::~TcpServer() = default;

bool TcpServer::is_socket_initialized() const
{
    return m_socket_fd.is_valid();
}

void TcpServer::start()
{
    if (!m_socket_fd.is_valid())
    {
        throw std::runtime_error("Socket not initialized");
    }

    if (bind(m_socket_fd.get(), reinterpret_cast<sockaddr*>(&m_server_addr), sizeof(m_server_addr)) < 0)
    {
        throw std::runtime_error("Failed to bind socket");
    }

    if (listen(m_socket_fd.get(), 5) < 0)
    {
        throw std::runtime_error("Failed to listen on socket");
    }
}
