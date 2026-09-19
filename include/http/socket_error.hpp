#pragma once

#include <string>
#include <system_error>

/// @brief Error codes for socket-related failures.
enum class SocketErrorCode
{
    /// @brief The socket is in an invalid state for the requested operation.
    invalid_state,

    /// @brief The specified port number is invalid.
    invalid_port,

    /// @brief A system-level error occurred.
    system_error,
};

/// @brief Represents an error encountered during socket operations.
struct SocketError
{
    /// @brief The error code indicating the type of socket error.
    SocketErrorCode code;

    /// @brief The underlying system error code, if applicable.
    std::error_code error_code{};

    /// @brief A diagnostic message providing additional details about the error.
    ///        Describes the operation that failed or the invalid input or state.
    std::string diagnostic;
};