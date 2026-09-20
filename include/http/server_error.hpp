#pragma once

#include <http/socket_error.hpp>

/// @brief Error codes for HTTP server failures.
enum class ServerErrorCode
{
    /// @brief Indicates a failure related to the underlying socket.
    socket_failure,

    /// @brief Indicates that the server is not currently running.
    not_running,

    /// @brief Indicates that the server is already running.
    already_running,
};

/// @brief Represents an error encountered by the HTTP server.
struct ServerError
{
    /// @brief The error code indicating the type of server error.
    ServerErrorCode code;

    /// @brief The socket error that caused the server operation to fail.
    SocketError socket_error;
};