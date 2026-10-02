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

/// @brief Represents the result of a receive operation on a TCP connection.
/// @details This structure contains the status of the receive operation and the number of bytes received.
enum class ReceiveResultStatus : std::uint8_t
{
    /// @brief Indicates that data was successfully received from the client.
    data_received,

    /// @brief Indicates that the peer has closed the connection.
    peer_closed,
};

/// @brief Represents the result of a receive operation on a TCP connection.
/// @details This structure contains the status of the receive operation and the number of bytes received.
struct ReceiveResult
{
    /// @brief The status of the receive operation.
    ReceiveResultStatus status;

    /// @brief The number of bytes received from the client.
    std::size_t bytes_received;
};

class TcpConnectionTestFactory;

/// @brief Represents a TCP connection with a client, providing access to the client's IP address,
/// port, and connection status.
class TcpConnection
{
public:
    /// @brief Retrieves the IP address of the connected client.
    /// @return The client's IP address as a string, or an error if the address could not be obtained.
    [[nodiscard]] std::expected<std::string, TcpConnectionError> get_client_ip_address() const;

    /// @brief Retrieves the port number of the connected client.
    /// @return The client's port number.
    std::uint16_t get_client_port() const;

    /// @brief Sends data to the connected client over the TCP connection.
    /// @param data The data to be sent to the client.
    /// @return `std::expected<void, TcpConnectionError>` indicating success or failure of the send operation.
    [[nodiscard]] std::expected<void, TcpConnectionError> send(std::span<const std::byte> data) const;

    /// @brief Receives data from the connected client over the TCP connection.
    /// @param buffer The buffer to store the received data.
    /// @return `std::expected<ReceiveResult, TcpConnectionError>` indicating the result of the receive operation.
    [[nodiscard]] std::expected<ReceiveResult, TcpConnectionError> receive(std::span<std::byte> buffer) const;

    /// @brief Checks if the TCP connection is valid.
    /// @return `true` if the connection is valid, `false` otherwise.
    [[nodiscard]] bool is_valid() const noexcept { return m_connected_client.is_valid(); }

    /// @brief Retrieves the underlying file descriptor for the connected client.
    /// @return The file descriptor as an integer.
    [[nodiscard]] int get() const noexcept { return m_connected_client.get(); }

private:
    TcpConnection(UniqueFileDescriptor fd, sockaddr_in ip_addr);
    UniqueFileDescriptor m_connected_client{};
    sockaddr_in m_client_endpoint{};

    friend class TcpSocket;
    friend class TcpConnectionTestFactory;
};
