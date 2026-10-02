#include <http_server/config/http_server_config_loader.hpp>
#include <http_server/tcp/server_error.hpp>
#include <http_server/tcp/tcp_server.hpp>

#include <filesystem>
#include <iostream>
#include <string_view>

namespace fs = std::filesystem;

using http_server::config::HttpServerConfigLoader;
using http_server::tcp::ServerError;
using http_server::tcp::ServerErrorCode;
using http_server::tcp::TcpServer;

static int run_server(const fs::path &config_path);

static std::string_view diagnostic(const ServerError &error)
{
    if (error.socket_error)
    {
        return error.socket_error->diagnostic;
    }

    switch (error.code)
    {
    case ServerErrorCode::socket_failure:
        return "Socket operation failed";
    case ServerErrorCode::not_running:
        return "Server is not running";
    case ServerErrorCode::already_running:
        return "Server is already running";
    }

    return "Unknown server error";
}

int main(int argument_count, char *arguments[])
{
    fs::path config_path{HttpServerConfigLoader::DEFAULT_CONFIG_PATH};

    if (argument_count == 3 && std::string_view{arguments[1]} == "--config")
    {
        config_path = arguments[2];
    }
    else if (argument_count != 1)
    {
        std::cerr << "Usage: " << arguments[0] << " [--config <path>]\n";
        return 1;
    }

    return run_server(config_path);
}

int run_server(const fs::path &config_path)
{
    HttpServerConfigLoader config_loader;

    if (const auto config_result = config_loader.load(config_path); !config_result)
    {
        std::cerr << "Failed to load configuration: " << config_result.error().diagnostic << "\n";
        return 1;
    }

    std::cout << "Server config loaded successfully\n";

    TcpServer server;

    if (const auto result = server.start(config_loader.config().tcp_server); !result)
    {
        std::cerr << "Failed to start server: " << diagnostic(result.error()) << "\n";
        return 1;
    }

    std::cout << "Server started successfully\n";

    if (const auto accept_result = server.accept_connection(); !accept_result)
    {
        std::cerr << "Failed to accept connection: " << diagnostic(accept_result.error()) << "\n";
        return 1;
    }

    std::cout << "Accepted connection successfully\n";

    server.stop();

    std::cout << "Server stopped successfully\n";
    return 0;
}
