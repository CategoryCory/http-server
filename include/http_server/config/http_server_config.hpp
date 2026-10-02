#pragma once

#include <http_server/tcp/tcp_server_config.hpp>

namespace http_server::config
{

/// @brief Application-wide configuration for the HTTP server.
struct HttpServerConfig
{
    /// @brief Configuration for the TCP server used by the application.
    tcp::TcpServerConfig tcp_server{};
};

} // namespace http_server::config
