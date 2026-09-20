#include <config/http_server_config_loader.hpp>
#include <http/tcp_server.hpp>

#include <filesystem>
#include <iostream>
#include <string_view>

namespace fs = std::filesystem;

static void test_start_twice();

static void test_accept_before_start();

static int test_full_lifecycle(const fs::path &config_path);

int main(int argument_count, char *arguments[])
{
    test_start_twice();
    test_accept_before_start();

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

    return test_full_lifecycle(config_path);
}

void test_start_twice()
{
    TcpServer server;

    if (const auto first_start_result = server.start({}); !first_start_result)
    {
        std::cerr << "Failed to start server the first time: " << first_start_result.error().socket_error.diagnostic
                  << "\n";
        return;
    }

    if (const auto second_start_result = server.start({}); !second_start_result)
    {
        std::cerr << "Failed to start server the second time: " << second_start_result.error().socket_error.diagnostic
                  << "\n";
    }

    server.stop();
}

void test_accept_before_start()
{
    TcpServer server;

    if (const auto accept_result = server.accept_connection(); !accept_result)
    {
        std::cerr << "Failed to accept connection before starting server: "
                  << accept_result.error().socket_error.diagnostic << "\n";
    }
}

int test_full_lifecycle(const fs::path &config_path)
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
        std::cerr << "Failed to start server: " << result.error().socket_error.diagnostic << "\n";
        return 1;
    }

    std::cout << "Server started successfully\n";

    if (const auto accept_result = server.accept_connection(); !accept_result)
    {
        std::cerr << "Failed to accept connection: " << accept_result.error().socket_error.diagnostic << "\n";
        return 1;
    }

    std::cout << "Accepted connection successfully\n";

    server.stop();

    std::cout << "Server stopped successfully\n";
    return 0;
}
