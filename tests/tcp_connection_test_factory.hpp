#pragma once

#include <http_server/core/unique_file_descriptor.hpp>
#include <http_server/tcp/tcp_connection.hpp>

#include <netinet/in.h>

class TcpConnectionTestFactory
{
public:
    static TcpConnection create(UniqueFileDescriptor fd, sockaddr_in client_address)
    {
        return TcpConnection(std::move(fd), client_address);
    }
};
