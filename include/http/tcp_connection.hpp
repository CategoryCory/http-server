#pragma once

#include <http/tcp_connection_error.hpp>
#include <http/unique_file_descriptor.hpp>

#include <arpa/inet.h>
#include <expected>
#include <netinet/in.h>
#include <span>
#include <string>

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
