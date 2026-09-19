#include <http/tcp_server.hpp>
#include <stdexcept>

std::expected<void, ServerError> TcpServer::start(const TcpServerConfig &server_config)
{
    if (const auto result = m_socket.initialize_socket(); !result)
    {
        return std::unexpected(ServerError{.code = ServerErrorCode::socket_failure, .socket_error = result.error()});
    }

    if (const auto result = m_socket.bind_address(server_config.port); !result)
    {
        return std::unexpected(ServerError{.code = ServerErrorCode::socket_failure, .socket_error = result.error()});
    }

    if (const auto result = m_socket.listen_for_connections(server_config.max_backlog); !result)
    {
        return std::unexpected(ServerError{.code = ServerErrorCode::socket_failure, .socket_error = result.error()});
    }

    return {};
}

void TcpServer::stop()
{
    throw std::runtime_error("TcpServer::stop() not yet implemented");
}
