#pragma once

#include <cstdint>
#include <string>
#include <system_error>

namespace http_server::tcp
{

/// @brief Error codes for socket-related failures.
enum class SocketErrorCode : std::uint8_t
{
    /// @brief The socket is in an invalid state for the requested operation.
    invalid_state,

    /// @brief The specified port number is invalid.
    invalid_port,

    /// @brief Configuring the socket failed.
    configuration_error,

    /// @brief A system-level error occurred.
    system_error,
};

/// @brief An error encountered during socket operations.
struct SocketError
{
    /// @brief The error code indicating the type of socket error.
    SocketErrorCode code;

    /// @brief The underlying system error code.
    ///
    /// Value-initialized (no error) when the failure did not originate from a system call.
    std::error_code error_code{};

    /// @brief A diagnostic message with additional details about the error.
    ///
    /// Describes the operation that failed or the invalid input or state.
    std::string diagnostic;
};

} // namespace http_server::tcp
