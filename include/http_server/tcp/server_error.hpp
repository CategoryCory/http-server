#pragma once

#include <http_server/tcp/socket_error.hpp>

#include <cstdint>
#include <optional>

namespace http_server::tcp
{

/// @brief Error codes for HTTP server failures.
enum class ServerErrorCode : std::uint8_t
{
    /// @brief Indicates a failure related to the underlying socket.
    socket_failure,

    /// @brief Indicates that the server is not currently running.
    not_running,

    /// @brief Indicates that the server is already running.
    already_running,
};

// TODO: Consider adding factory functions for creating common server errors.
/// @brief Represents an error encountered by the HTTP server.
struct ServerError
{
    /// @brief The error code indicating the type of server error.
    ServerErrorCode code;

    /// @brief The socket error that caused the failure, when @ref code is socket_failure.
    std::optional<SocketError> socket_error;
};

} // namespace http_server::tcp
