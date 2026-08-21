#include <core/tcp_server.hpp>

using namespace std::literals;

TcpServer::TcpServer()
{
    if (m_socket_fd != -1)
    {
        throw std::runtime_error("Socket already initialized");
    }
    
    m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket_fd < 0)
    {
        throw std::runtime_error("Failed to create socket");
    }

    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    m_server_addr.sin_port = htons(DEFAULT_PORT);
}

TcpServer::~TcpServer()
{
    if (m_socket_fd != -1)
    {
        close(m_socket_fd);
    }
    
    m_socket_fd = -1;
}

std::string_view TcpServer::get_version() const
{
    return "1.0.0"sv;
}
