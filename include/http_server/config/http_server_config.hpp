#pragma once

#include <http_server/tcp/tcp_server_config.hpp>

/// @brief Application-wide configuration for the HTTP server.
struct HttpServerConfig
{
    /// @brief Configuration for the TCP server used by the application.
    TcpServerConfig tcp_server{};
};
