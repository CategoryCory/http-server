#include <core/tcp_server.hpp>
#include <iostream>

int main() {
    TcpServer server;
    std::cout << "TCP server initialized: " << (server.is_socket_initialized() ? "yes" : "no") << "\n";
    return 0;
}
