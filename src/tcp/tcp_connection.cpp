#include <http_server/core/unique_file_descriptor.hpp>
#include <http_server/tcp/tcp_connection.hpp>
#include <http_server/tcp/tcp_connection_error.hpp>

#include "platform.hpp"

#include <arpa/inet.h>
#include <array>
#include <cerrno>
#include <cstddef>
#include <expected>
#include <netinet/in.h>
#include <span>
#include <string>
#include <sys/socket.h>
#include <system_error>
#include <utility>

namespace http_server::tcp
{

TcpConnection::TcpConnection(core::UniqueFileDescriptor &&fd, const sockaddr_in ip_addr)
    : m_connected_client{std::move(fd)},
      m_client_endpoint(ip_addr)
{
}

std::expected<std::string, TcpConnectionError> TcpConnection::get_client_ip_address() const
{
    std::array<char, INET_ADDRSTRLEN> buffer{};

    if (const char *result = ::inet_ntop(AF_INET, &m_client_endpoint.sin_addr, buffer.data(), buffer.size());
        result == nullptr)
    {
        return std::unexpected(TcpConnectionError{
            .code = TcpConnectionErrorCode::ip_addr_conversion_failure,
        });
    }

    return buffer.data();
}

std::uint16_t TcpConnection::get_client_port() const
{
    return ntohs(m_client_endpoint.sin_port);
}

std::expected<void, TcpConnectionError> TcpConnection::send(std::span<const std::byte> data) const
{
    std::size_t total_bytes_sent = 0;

    while (total_bytes_sent < data.size_bytes())
    {
        const auto data_remaining = data.last(data.size_bytes() - total_bytes_sent);

        const auto bytes_sent =
            ::send(m_connected_client.get(), data_remaining.data(), data_remaining.size_bytes(), TCP_SEND_FLAGS);

        if (bytes_sent < 0)
        {
            // TODO: Add ECONNRESET and ENOTCONN
            switch (const int error_code = errno)
            {
            // Operation interrupted; continue
            case EINTR:
                continue;

            case EPIPE:
                return std::unexpected(TcpConnectionError{
                    .code = TcpConnectionErrorCode::send_client_unavailable,
                    .error_code = {error_code, std::generic_category()},
                });

            // Default error handler
            default:
                return std::unexpected(TcpConnectionError{
                    .code = TcpConnectionErrorCode::send_general_failure,
                    .error_code = {error_code, std::generic_category()},
                });
            }
        }

        if (bytes_sent == 0)
        {
            return std::unexpected(TcpConnectionError{
                .code = TcpConnectionErrorCode::send_no_progress,
            });
        }

        total_bytes_sent += static_cast<std::size_t>(bytes_sent);
    }

    return {};
}

std::expected<ReceiveResult, TcpConnectionError> TcpConnection::receive(std::span<std::byte> buffer) const
{
    if (buffer.empty())
    {
        return std::unexpected(TcpConnectionError{
            .code = TcpConnectionErrorCode::recv_no_buffer,
        });
    }

    for (;;)
    {
        const auto bytes_received = ::recv(m_connected_client.get(), buffer.data(), buffer.size_bytes(), 0);

        if (bytes_received < 0)
        {
            // TODO: Expand error handling
            switch (const auto error_code = errno)
            {
            case EINTR:
                continue;
            default:
                return std::unexpected(TcpConnectionError{
                    .code = TcpConnectionErrorCode::recv_general_failure,
                    .error_code = {error_code, std::generic_category()},
                });
            }
        }

        if (bytes_received == 0)
        {
            return ReceiveResult{
                .status = ReceiveResultStatus::peer_closed,
                .bytes_received = 0,
            };
        }

        return ReceiveResult{
            .status = ReceiveResultStatus::data_received,
            .bytes_received = static_cast<std::size_t>(bytes_received),
        };
    }
}

} // namespace http_server::tcp
