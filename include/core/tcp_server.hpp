#pragma once

#include <netinet/in.h>

/// @brief Default port number for the TCP server
constexpr int DEFAULT_PORT = 8080;

/// @brief A TCP server that listens for incoming client connections
/// 
/// This class manages a TCP socket bound to a local address and port,
/// allowing it to accept incoming client connections. It is designed
/// to be used as a building block for higher-level network protocols
/// such as HTTP servers.
/// 
/// The class is move-enabled but non-copyable to prevent accidental
/// duplication of socket resources. Socket resources are properly cleaned
/// up via RAII principles in the destructor.
class TcpServer
{
public:
    /// @brief Checks if the internal socket has been successfully initialized
    /// @return true if the socket was created successfully, false otherwise
    [[nodiscard]] bool is_socket_initialized() const;
    
    /// @brief Starts the server by binding to the configured address and listening for connections
    /// @throws std::runtime_error if the socket is not initialized, binding fails, or listening fails
    void start();
    
    /// @brief Constructs a new TCP server and creates an unbound socket
    /// @throws std::runtime_error if socket creation fails
    TcpServer();
    
    /// @brief Copy construction is deleted (sockets cannot be safely copied)
    TcpServer(const TcpServer&) = delete;
    
    /// @brief Copy assignment is deleted (sockets cannot be safely copied)
    TcpServer& operator=(const TcpServer&) = delete;
    
    /// @brief Move constructor that transfers socket ownership
    /// @param other The TcpServer instance to move from (relinquishes ownership of its socket)
    TcpServer(TcpServer&&) noexcept;
    
    /// @brief Move assignment that transfers socket ownership and cleans up any existing socket
    /// @param other The TcpServer instance to move from (relinquishes ownership of its socket)
    /// @return Reference to this TcpServer instance
    TcpServer& operator=(TcpServer&&) noexcept;
    
    /// @brief Destructor that closes the socket if it is open
    ~TcpServer();
private:
    /// @brief File descriptor for the server socket (-1 if not initialized)
    int m_socket_fd = -1;
    
    /// @brief Socket address structure containing the server's bind address and port
    sockaddr_in m_server_addr{};
};
