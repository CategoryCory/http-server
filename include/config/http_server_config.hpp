#pragma once

#include <http/tcp_server.hpp>

/// @brief Application-wide configuration for the HTTP server.
struct HttpServerConfig
{
    /// @brief Configuration for the TCP server used by the application.
    TcpServerConfig tcp_server{};
};