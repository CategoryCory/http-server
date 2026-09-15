#include <http/result.hpp>
#include <http/tcp_server.hpp>

#include <iostream>

int main() {
    TcpServer server;

    auto config = TcpServerConfig{
        .port = 8080,
        .max_backlog = 5
    };
    
    auto result = server.start(config);

    std::cout << "Server start result: " << (result.is_success() ? "success" : "failure") << "\n";
    return 0;
}
