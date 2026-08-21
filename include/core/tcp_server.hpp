#pragma once

#include <string_view>
#include <netinet/in.h>
#include <unistd.h>

const int DEFAULT_PORT = 8080;

class TcpServer
{
public:
    std::string_view get_version() const;
    TcpServer();
    ~TcpServer();
private:
    int m_socket_fd = -1;
    struct sockaddr_in m_server_addr;
};
