#pragma once

#include <http_server/core/unique_file_descriptor.hpp>
#include <http_server/tcp/tcp_connection_error.hpp>

#include <arpa/inet.h>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <netinet/in.h>
#include <span>
#include <string>

namespace http_server::tcp
{

/// @brief The outcome of a successful receive operation on a TCP connection.
enum class ReceiveResultStatus : std::uint8_t
{
    /// @brief Data was received from the client.
    data_received,

    /// @brief The peer closed the connection.
    peer_closed,
};

/// @brief The result of a receive operation on a TCP connection.
struct ReceiveResult
{
    /// @brief The status of the receive operation.
    ReceiveResultStatus status;

    /// @brief The number of bytes received from the client.
    ///
    /// Zero when @ref status is ReceiveResultStatus::peer_closed.
    std::size_t bytes_received;
};

class TcpConnectionTestFactory;

/// @brief A TCP connection with a client.
///
/// Provides access to the client's address and sends and receives data over the connection. Instances are created by
/// TcpListener and own the connected socket.
class TcpConnection
{
public:
    /// @brief Returns the IP address of the connected client.
    /// @return A std::expected containing the client's IP address in dotted-decimal form on success;
    ///         otherwise an unexpected TcpConnectionError.
    [[nodiscard]] std::expected<std::string, TcpConnectionError> get_client_ip_address() const;

    /// @brief Returns the port number of the connected client.
    /// @return The client's port number in host byte order.
    std::uint16_t get_client_port() const;

    /// @brief Sends data to the connected client.
    ///
    /// Blocks until all of @p data has been sent or an error occurs.
    /// @param data The data to send to the client.
    /// @return An engaged std::expected when all data was sent; otherwise an unexpected TcpConnectionError.
    [[nodiscard]] std::expected<void, TcpConnectionError> send(std::span<const std::byte> data) const;

    /// @brief Receives data from the connected client.
    ///
    /// Blocks until data is available, the peer closes the connection, or an error occurs.
    /// @param buffer The buffer to store the received data; must not be empty.
    /// @return A std::expected containing the ReceiveResult on success; otherwise an unexpected TcpConnectionError.
    [[nodiscard]] std::expected<ReceiveResult, TcpConnectionError> receive(std::span<std::byte> buffer) const;

    /// @brief Checks whether the connection owns a valid socket descriptor.
    /// @return true when the connection is valid; otherwise false.
    [[nodiscard]] bool is_valid() const noexcept { return m_connected_client.is_valid(); }

    /// @brief Returns the underlying file descriptor of the connected client.
    /// @return The socket descriptor, or -1 if the connection is invalid.
    [[nodiscard]] int get() const noexcept { return m_connected_client.get(); }

private:
    TcpConnection(core::UniqueFileDescriptor &&fd, sockaddr_in ip_addr);
    core::UniqueFileDescriptor m_connected_client{};
    sockaddr_in m_client_endpoint{};

    friend class TcpListener;
    friend class TcpConnectionTestFactory;
};

} // namespace http_server::tcp
