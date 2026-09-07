#pragma once

#include <http/unique_file_descriptor.hpp>
#include <netinet/in.h>

/// @brief Manages an IPv4 TCP socket and its bind address.
///
/// Owns its underlying POSIX file descriptor and configures sockets for address
/// reuse before binding them to a local port.
class TcpSocket
{
  public:
    /// @brief Creates and configures an IPv4 TCP socket.
    ///
    /// Replaces any socket currently owned by this instance only after the new
    /// socket has been configured successfully.
    /// @throws std::system_error if the socket cannot be created or configured.
    void initialize_socket();

    /// @brief Binds the initialized socket to all local IPv4 interfaces.
    /// @param port The local port number in the range 0-65535.
    /// @throws std::invalid_argument if @p port is outside the valid range.
    /// @throws std::logic_error if the socket has not been initialized.
    /// @throws std::system_error if the socket cannot be bound.
    void bind_address(int port);

    /// @brief Checks whether this instance owns an initialized socket.
    /// @return true when an initialized socket is owned; otherwise false.
    [[nodiscard]] bool is_valid() const noexcept;

    /// @brief Returns the owned socket descriptor without transferring ownership.
    /// @return The socket descriptor, or -1 if no socket is owned.
    [[nodiscard]] int get() const noexcept;
  private:
    UniqueFileDescriptor m_socket_fd{};
    sockaddr_in m_server_addr{};
};
  