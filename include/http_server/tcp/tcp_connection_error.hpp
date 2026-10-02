#pragma once

#include <cstdint>
#include <system_error>

/// @brief Error codes for TCP connection failures.
enum class TcpConnectionErrorCode : std::uint8_t
{
    /// @brief Failed to convert IP address to the required format.
    ip_addr_conversion_failure,

    /// @brief Send returned zero bytes sent.
    send_no_progress,

    /// @brief The connected client was not available.
    send_client_unavailable,

    /// @brief Failed to send data.
    send_general_failure,

    /// @brief Indicates that the `receive` buffer had a capacity of zero.
    recv_no_buffer,

    /// @brief Failed to receive data.
    recv_general_failure,
};

/// @brief Represents an error that occurred during a TCP connection attempt.
struct TcpConnectionError
{
    /// @brief The specific error code for the TCP connection failure.
    TcpConnectionErrorCode code;

    /// @brief The underlying system error code, if applicable.
    std::error_code error_code{};
};
