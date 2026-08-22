#include <netinet/in.h>
#include <stdexcept>
#include <unistd.h>
#include <utility>
#include <core/tcp_server.hpp>

/// @brief Creates a socket and initializes the server address configuration
/// 
/// The socket is created with IPv4 (AF_INET) and TCP protocol (SOCK_STREAM).
/// The server address is configured to accept connections on any available
/// interface (INADDR_ANY) on the DEFAULT_PORT.
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

/// @brief Move constructor that safely transfers socket ownership
/// 
/// The source object's socket file descriptor and server address are transferred
/// to this object, and the source object is left in an uninitialized state
/// (socket_fd = -1) to prevent double-closing.
TcpServer::TcpServer(TcpServer&& other) noexcept
    : m_socket_fd(std::exchange(other.m_socket_fd, -1)),
      m_server_addr(std::exchange(other.m_server_addr, sockaddr_in{})) { }

/// @brief Move assignment operator that transfers socket ownership
/// 
/// If this object already holds a socket, it will be closed before
/// transferring ownership from the source object. The source object is
/// left in an uninitialized state (socket_fd = -1).
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

/// @brief Closes the socket if it is open
/// 
/// This destructor ensures proper cleanup of the socket resource.
/// If the socket file descriptor is valid (not -1), it is closed.
TcpServer::~TcpServer()
{
    if (m_socket_fd != -1)
    {
        close(m_socket_fd);
    }
}

/// @brief Checks if the internal socket has been successfully initialized
/// @return true if m_socket_fd is valid (not -1), false otherwise
bool TcpServer::is_socket_initialized() const
{
    return m_socket_fd != -1;
}

/// @brief Binds the socket to the configured address and starts listening for incoming connections
/// 
/// This method performs two key operations:
/// 1. Binds the socket to the server address (INADDR_ANY on DEFAULT_PORT)
/// 2. Marks the socket as listening with a backlog queue of 5 connections
/// 
/// @throws std::runtime_error if the socket is not initialized
/// @throws std::runtime_error if socket binding fails
/// @throws std::runtime_error if listening fails
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
