#include <http/tcp_server.hpp>
#include <http/tcp_socket.hpp>

#include <stdexcept>
#include <utility>

std::expected<void, ServerError> TcpServer::start(const TcpServerConfig &server_config)
{
    if (m_state == TcpServerState::Running)
    {
        return std::unexpected(ServerError{.code = ServerErrorCode::already_running});
    }

    TcpSocket temp_socket{};

    if (const auto result = temp_socket.initialize_socket(); !result)
    {
        return std::unexpected(ServerError{.code = ServerErrorCode::socket_failure, .socket_error = result.error()});
    }

    if (const auto result = temp_socket.bind_address(server_config.port); !result)
    {
        return std::unexpected(ServerError{.code = ServerErrorCode::socket_failure, .socket_error = result.error()});
    }

    if (const auto result = temp_socket.listen_for_connections(server_config.max_backlog); !result)
    {
        return std::unexpected(ServerError{.code = ServerErrorCode::socket_failure, .socket_error = result.error()});
    }

    m_socket = std::move(temp_socket);
    m_state = TcpServerState::Running;
    return {};
}

std::expected<TcpSocket, ServerError> TcpServer::accept_connection()
{
    if (m_state != TcpServerState::Running)
    {
        return std::unexpected(ServerError{.code = ServerErrorCode::not_running});
    }

    auto result = m_socket.accept_connection();
    if (!result)
    {
        return std::unexpected(ServerError{.code = ServerErrorCode::socket_failure, .socket_error = result.error()});
    }

    return std::move(result.value());
}

void TcpServer::stop() noexcept
{
    if (m_state == TcpServerState::Running)
    {
        m_socket.close();
        m_state = TcpServerState::Stopped;
    }
}
