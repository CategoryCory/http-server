#include <http_server/tcp/tcp_server.hpp>

#include <http_server/tcp/tcp_listener.hpp>

#include <expected>
#include <optional>
#include <utility>

namespace http_server::tcp
{

std::expected<void, ServerError> TcpServer::start(const TcpServerConfig &server_config)
{
    if (m_listener.has_value())
    {
        return std::unexpected(ServerError{
            .code = ServerErrorCode::already_running,
            .socket_error = std::nullopt,
        });
    }

    return TcpListener::create(server_config.port, server_config.max_backlog)
        .transform([&](TcpListener listener) {
            m_listener = std::move(listener);
        })
        .transform_error([](SocketError socket_error) {
            return ServerError{
                .code = ServerErrorCode::socket_failure,
                .socket_error = std::move(socket_error),
            };
        });
}

std::expected<TcpConnection, ServerError> TcpServer::accept_connection() const
{
    if (!m_listener.has_value())
    {
        return std::unexpected(ServerError{
            .code = ServerErrorCode::not_running,
            .socket_error = std::nullopt,
        });
    }

    auto result = m_listener->accept_connection();
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
    if (m_listener.has_value())
    {
        m_listener->close();
        m_listener.reset();
    }
}

} // namespace http_server::tcp
