#pragma once

#include <expected>
#include <http/socket_error.hpp>
#include <http/unique_file_descriptor.hpp>
#include <netinet/in.h>

/// @brief Describes the outcome of a future connection operation.
/// @note This type is not used by the current API.
enum class ConnectionResult : std::uint8_t
{
    /// @brief The operation completed successfully.
    Success,

    /// @brief The operation timed out.
    Timeout,

    /// @brief The operation failed.
    Error
};

/// @brief Describes the lifecycle state of a TCP socket.
enum class TcpSocketState : std::uint8_t
{
    /// @brief No socket descriptor is owned.
    Uninitialized,

    /// @brief A socket descriptor has been created and configured.
    Initialized,

    /// @brief The socket is bound to a local address.
    Bound,

    /// @brief The socket is accepting incoming connections.
    Listening,

    /// @brief The socket represents an accepted client connection.
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
    /// Initializes the socket and prepares it for binding and listening.
    /// @return An engaged std::expected on success; otherwise an unexpected SocketError.
    [[nodiscard]] std::expected<void, SocketError> initialize_socket();

    /// @brief Binds the initialized socket to all local IPv4 interfaces.
    /// @param port The local port number in the range 0-65535.
    /// @return An engaged std::expected on success; otherwise an unexpected SocketError.
    [[nodiscard]] std::expected<void, SocketError> bind_address(int port);

    /// @brief Listens for incoming connections on the bound socket.
    /// @param max_backlog The maximum number of pending connections in the queue.
    /// @return An engaged std::expected on success; otherwise an unexpected SocketError.
    [[nodiscard]] std::expected<void, SocketError> listen_for_connections(int max_backlog);

    /// @brief Accepts an incoming connection on the listening socket.
    /// @return An engaged std::expected containing a connected TcpSocket on success;
    ///         otherwise an unexpected SocketError.
    [[nodiscard]] std::expected<TcpSocket, SocketError> accept_connection() const;

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
    [[nodiscard]] std::expected<void, SocketError> require_socket_state(TcpSocketState required_state) const;
};
