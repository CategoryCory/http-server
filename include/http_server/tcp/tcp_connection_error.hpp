#pragma once

#include <cstdint>
#include <system_error>

namespace http_server::tcp
{

/// @brief Error codes for TCP connection failures.
enum class TcpConnectionErrorCode : std::uint8_t
{
    /// @brief Converting the client IP address to text failed.
    ip_addr_conversion_failure,

    /// @brief A send operation made no progress because zero bytes were sent.
    send_no_progress,

    /// @brief A send operation failed because the client is no longer reachable.
    send_client_unavailable,

    /// @brief A send operation failed for another reason.
    send_general_failure,

    /// @brief A receive operation was requested with an empty buffer.
    recv_no_buffer,

    /// @brief A receive operation failed.
    recv_general_failure,
};

/// @brief An error encountered during a TCP connection operation.
struct TcpConnectionError
{
    /// @brief The error code indicating the type of TCP connection error.
    TcpConnectionErrorCode code;

    /// @brief The underlying system error code.
    ///
    /// Value-initialized (no error) when the failure did not originate from a system call.
    std::error_code error_code{};
};

} // namespace http_server::tcp
