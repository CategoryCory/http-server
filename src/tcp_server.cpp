#include <http/tcp_server.hpp>

#include <expected>
#include <optional>
#include <utility>

std::expected<void, ServerError> TcpServer::start(const TcpServerConfig &server_config)
{
    if (m_state == TcpServerState::Running)
    {
        return std::unexpected(ServerError{
            .code = ServerErrorCode::already_running,
            .socket_error = std::nullopt,
        });
    }

    TcpSocket temp_socket{};

    return temp_socket.initialize_socket()
        .and_then(
            [&]
            {
                return temp_socket.bind_address(server_config.port);
            })
        .and_then(
            [&]
            {
                return temp_socket.listen_for_connections(server_config.max_backlog);
            })
        .transform(
            [&]() noexcept
            {
                m_socket = std::move(temp_socket);
                m_state = TcpServerState::Running;
            })
        .transform_error(
            [](SocketError error)
            {
                return ServerError{
                    .code = ServerErrorCode::socket_failure,
                    .socket_error = std::move(error),
                };
            });
}

std::expected<TcpSocket, ServerError> TcpServer::accept_connection() const
{
    if (m_state != TcpServerState::Running)
    {
        return std::unexpected(ServerError{
            .code = ServerErrorCode::not_running,
            .socket_error = std::nullopt,
        });
    }

    auto result = m_socket.accept_connection();
    if (!result)
    {
        return std::unexpected(ServerError{
            .code = ServerErrorCode::socket_failure,
            .socket_error = result.error(),
        });
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
