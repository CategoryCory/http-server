#pragma once

#include <http/tcp_connection_error.hpp>
#include <http/unique_file_descriptor.hpp>

#include <arpa/inet.h>
#include <expected>
#include <netinet/in.h>
#include <string>

class TcpConnectionTestFactory;

class TcpConnection
{
public:
    [[nodiscard]] std::expected<std::string, TcpConnectionError> get_client_ip_address() const;
    std::uint16_t get_client_port() const;

private:
    TcpConnection(UniqueFileDescriptor fd, sockaddr_in ip_addr);
    UniqueFileDescriptor m_connected_client{};
    sockaddr_in m_client_endpoint{};

    friend class TcpSocket;
    friend class TcpConnectionTestFactory;
};
