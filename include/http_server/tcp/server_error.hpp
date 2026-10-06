#pragma once

#include <http_server/tcp/socket_error.hpp>

#include <cstdint>
#include <optional>

namespace http_server::tcp
{

/// @brief Error codes for TCP server failures.
enum class ServerErrorCode : std::uint8_t
{
    /// @brief A failure occurred in the underlying socket.
    socket_failure,

    /// @brief The server is not currently running.
    not_running,

    /// @brief The server is already running.
    already_running,
};

// TODO: Consider adding factory functions for creating common server errors.
/// @brief An error encountered by the TCP server.
struct ServerError
{
    /// @brief The error code indicating the type of server error.
    ServerErrorCode code;

    /// @brief The socket error that caused the failure.
    ///
    /// Engaged only when @ref code is ServerErrorCode::socket_failure.
    std::optional<SocketError> socket_error;
};

} // namespace http_server::tcp
