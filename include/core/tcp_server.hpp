#pragma once

#include <netinet/in.h>

constexpr int DEFAULT_PORT = 8080;

class TcpServer
{
public:
    [[nodiscard]] bool is_socket_initialized() const;
    void start();
    TcpServer();
    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;
    TcpServer(TcpServer&&) noexcept;
    TcpServer& operator=(TcpServer&&) noexcept;
    ~TcpServer();
private:
    int m_socket_fd = -1;
    sockaddr_in m_server_addr{};
};
