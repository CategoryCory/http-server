#include <core/tcp_server.hpp>
#include <iostream>

int main() {
    TcpServer server;
    std::cout << "TCP Server Version: " << server.get_version() << "\n";
    return 0;
}
