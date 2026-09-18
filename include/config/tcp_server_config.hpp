#pragma once

#include <cstdint>

/// @brief Configuration structure for the TCP server
struct TcpServerConfig
{
    /// @brief Port number the server will listen on
    std::uint16_t port{};

    /// @brief Maximum number of pending connections the server can have in its listen queue
    int max_backlog{};
};
