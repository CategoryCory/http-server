#include <config/http_server_config_loader.hpp>
#include <http/tcp_server.hpp>

#include <filesystem>
#include <iostream>
#include <string_view>

int main(int argument_count, char *arguments[])
{
    std::filesystem::path config_path{HttpServerConfigLoader::DEFAULT_CONFIG_PATH};

    if (argument_count == 3 && std::string_view{arguments[1]} == "--config")
    {
        config_path = arguments[2];
    }
    else if (argument_count != 1)
    {
        std::cerr << "Usage: " << arguments[0] << " [--config <path>]\n";
        return 1;
    }

    HttpServerConfigLoader config_loader;
    const auto config_result = config_loader.load(config_path);

    if (!config_result)
    {
        std::cerr << "Failed to load configuration: " << config_result.error().diagnostic << "\n";
        return 1;
    }

    std::cout << "Server config loaded successfully\n";

    TcpServer server;
    const auto result = server.start(config_loader.config().tcp_server);

    if (!result)
    {
        std::cerr << "Failed to start server: " << result.error().socket_error.diagnostic << "\n";
        return 1;
    }

    std::cout << "Server started successfully\n";

    const auto accept_result = server.accept_connection();

    if (!accept_result)
    {
        std::cerr << "Failed to accept connection: " << accept_result.error().socket_error.diagnostic << "\n";
        return 1;
    }

    std::cout << "Accepted connection successfully\n";

    server.stop();

    std::cout << "Server stopped successfully\n";
    return 0;
}
