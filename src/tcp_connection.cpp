#include <http/platform.hpp>
#include <http/tcp_connection.hpp>
#include <http/tcp_connection_error.hpp>
#include <http/unique_file_descriptor.hpp>

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

TcpConnection::TcpConnection(UniqueFileDescriptor fd, const sockaddr_in ip_addr)
    : m_connected_client{std::move(fd)},
      m_client_endpoint(ip_addr)
{
}

std::expected<std::string, TcpConnectionError> TcpConnection::get_client_ip_address() const
{
    std::array<char, INET_ADDRSTRLEN> buffer{};

    const char *result = ::inet_ntop(AF_INET, &m_client_endpoint.sin_addr, buffer.data(), buffer.size());

    if (result == nullptr)
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

std::expected<void, TcpConnectionError> TcpConnection::send(const std::span<const std::byte> data) const
{
    std::size_t total_bytes_sent = 0;

    while (total_bytes_sent < data.size_bytes())
    {
        auto data_remaining = data.last(data.size_bytes() - total_bytes_sent);

        if (const auto bytes_sent =
                ::send(m_connected_client.get(), data_remaining.data(), data_remaining.size_bytes(), TCP_SEND_FLAGS);
            bytes_sent < 0)
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
        else if (bytes_sent == 0)
        {
            return std::unexpected(TcpConnectionError{
                .code = TcpConnectionErrorCode::send_no_progress,
            });
        }
        else
        {
            total_bytes_sent += static_cast<std::size_t>(bytes_sent);
        }
    }

    return {};
}

std::expected<ReceiveResult, TcpConnectionError> TcpConnection::receive(std::span<std::byte> buffer)
{
    if (buffer.size_bytes() == 0)
    {
        return std::unexpected(TcpConnectionError{
            .code = TcpConnectionErrorCode::recv_no_buffer,
        });
    }

    const auto bytes_received = ::recv(m_connected_client.get(), buffer.data(), buffer.size_bytes(), 0);

    if (bytes_received < 0)
    {
        // TODO: expand error handling
        return std::unexpected(TcpConnectionError{
            .code = TcpConnectionErrorCode::recv_general_failure,
        });
    }
    else if (bytes_received == 0)
    {
        return ReceiveResult{
            .status = ReceiveResultStatus::peer_closed,
            .bytes_received = 0,
        };
    }
    else
    {
        return ReceiveResult{
            .status = ReceiveResultStatus::data_received,
            .bytes_received = static_cast<std::size_t>(bytes_received),
        };
    }
}
