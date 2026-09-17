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
    const Result config_result = config_loader.load(config_path);

    if (!config_result.is_success())
    {
        std::cerr << "Failed to load configuration: " << config_result.error_message() << "\n";
        return 1;
    }

    TcpServer server;
    const Result result = server.start(config_loader.config().tcp_server);

    std::cout << "Server start result: " << (result.is_success() ? "success" : "failure") << "\n";
    return result.is_success() ? 0 : 1;
}
