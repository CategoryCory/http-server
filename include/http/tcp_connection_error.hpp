#pragma once

#include <cstdint>

/// @brief Error codes for TCP connection failures.
enum class TcpConnectionErrorCode : std::uint8_t
{
    /// @brief Failed to convert IP address to the required format.
    ip_addr_conversion_failure,
};

/// @brief Represents an error that occurred during a TCP connection attempt.
struct TcpConnectionError
{
    /// @brief The specific error code for the TCP connection failure.
    TcpConnectionErrorCode code;
};
