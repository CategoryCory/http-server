#pragma once

#include <http/result.hpp>
#include <http/unique_file_descriptor.hpp>

#include <netinet/in.h>

enum class ConnectionResult
{
    Success,
    Timeout,
    Error
};

enum class TcpSocketState
{
    Uninitialized,
    Initialized,
    Bound,
    Listening,
    Connected
};

/// @brief Manages an IPv4 TCP socket and its bind address.
///
/// Owns its underlying POSIX file descriptor and configures sockets for address
/// reuse before binding them to a local port.
class TcpSocket
{
  public:
    /// @brief Default constructor. Creates an uninitialized TcpSocket instance.
    TcpSocket() = default;

    /// @brief Creates and configures an IPv4 TCP socket.
    ///
    /// Replaces any socket currently owned by this instance only after the new
    /// socket has been configured successfully.
    /// @throws std::logic_error if the socket has already been initialized.
    /// @throws std::system_error if the socket cannot be created or configured.
    void initialize_socket();

    /// @brief Binds the initialized socket to all local IPv4 interfaces.
    /// @param port The local port number in the range 0-65535.
    /// @throws std::invalid_argument if @p port is outside the valid range.
    /// @throws std::logic_error if the socket has not been initialized.
    /// @throws std::system_error if the socket cannot be bound.
    void bind_address(int port);

    /// @brief Listens for incoming connections on the bound socket.
    /// @param max_pending_connections The maximum number of pending connections in the queue.
    /// @throws std::logic_error if the socket has not been initialized or bound.
    /// @throws std::system_error if the socket cannot be set to listen.
    void listen_for_connections(int max_pending_connections);

    /// @brief Accepts an incoming connection on the listening socket.
    /// @return A new TcpSocket instance representing the accepted connection.
    /// @throws std::logic_error if the socket has not been initialized or set to listen.
    /// @throws std::system_error if the connection cannot be accepted.
    TcpSocket accept_connection();

    /// @brief Checks whether this instance owns an initialized socket.
    /// @return true when an initialized socket is owned; otherwise false.
    [[nodiscard]] bool is_valid() const noexcept;

    /// @brief Returns the owned socket descriptor without transferring ownership.
    /// @return The socket descriptor, or -1 if no socket is owned.
    [[nodiscard]] int get() const noexcept;

  private:
    TcpSocket(UniqueFileDescriptor &&socket_fd, TcpSocketState state);
    UniqueFileDescriptor m_socket_fd{};
    sockaddr_in m_addr{};
    TcpSocketState m_state{TcpSocketState::Uninitialized};
};
