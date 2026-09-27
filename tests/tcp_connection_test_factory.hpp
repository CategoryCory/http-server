#pragma once

#include <http/tcp_connection.hpp>
#include <http/unique_file_descriptor.hpp>

#include <netinet/in.h>

class TcpConnectionTestFactory
{
public:
    static TcpConnection create(UniqueFileDescriptor fd, sockaddr_in client_address)
    {
        return TcpConnection(std::move(fd), client_address);
    }
};
