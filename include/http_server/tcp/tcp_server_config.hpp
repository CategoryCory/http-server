#pragma once

#include <cstdint>

namespace http_server::tcp
{

/// @brief Configuration for the TCP server.
struct TcpServerConfig
{
    /// @brief The port number the server listens on.
    std::uint16_t port{};

    /// @brief The maximum number of pending connections in the server's listen queue.
    int max_backlog{};
};

} // namespace http_server::tcp
