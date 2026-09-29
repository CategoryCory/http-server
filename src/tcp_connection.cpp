#include <http/tcp_connection.hpp>
#include <http/tcp_connection_error.hpp>
#include <http/unique_file_descriptor.hpp>

#include <arpa/inet.h>
#include <array>
#include <expected>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <utility>

TcpConnection::TcpConnection(UniqueFileDescriptor fd, sockaddr_in ip_addr)
    : m_connected_client{std::move(fd)},
      m_client_endpoint(ip_addr)
{
}

std::expected<std::string, TcpConnectionError> TcpConnection::get_client_ip_address() const
{
    std::array<char, INET_ADDRSTRLEN> buffer{};

    const char *result =
        ::inet_ntop(AF_INET, &m_client_endpoint.sin_addr, buffer.data(), static_cast<socklen_t>(buffer.size()));

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
