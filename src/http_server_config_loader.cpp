#include <config/http_server_config_loader.hpp>
#include <exception>
#include <expected>
#include <limits>
#include <toml++/toml.hpp>
#include <utility>

std::expected<void, ConfigError> HttpServerConfigLoader::load()
{
    return load(std::filesystem::path{DEFAULT_CONFIG_PATH});
}

std::expected<void, ConfigError> HttpServerConfigLoader::load(const std::filesystem::path &config_path)
{
    m_config = {};
    m_is_loaded = false;

    try
    {
        const toml::table toml_config = toml::parse_file(config_path.string());
        const auto port_node = toml_config.at_path("tcp_server.port");
        const auto max_backlog_node = toml_config.at_path("tcp_server.max_backlog");

        if (!port_node)
        {
            return std::unexpected(ConfigError{.code = ConfigErrorCode::missing_required_value,
                                               .config_path = config_path,
                                               .key = "tcp_server.port",
                                               .diagnostic = "Missing required configuration value tcp_server.port"});
        }

        if (!max_backlog_node)
        {
            return std::unexpected(
                ConfigError{.code = ConfigErrorCode::missing_required_value,
                            .config_path = config_path,
                            .key = "tcp_server.max_backlog",
                            .diagnostic = "Missing required configuration value tcp_server.max_backlog"});
        }

        const auto port = port_node.value<std::int64_t>();
        const auto max_backlog = max_backlog_node.value<std::int64_t>();

        if (!port)
        {
            return std::unexpected(ConfigError{.code = ConfigErrorCode::invalid_value,
                                               .config_path = config_path,
                                               .key = "tcp_server.port",
                                               .diagnostic = "Configuration value tcp_server.port must be an integer"});
        }

        if (!max_backlog)
        {
            return std::unexpected(
                ConfigError{.code = ConfigErrorCode::invalid_value,
                            .config_path = config_path,
                            .key = "tcp_server.max_backlog",
                            .diagnostic = "Configuration value tcp_server.max_backlog must be an integer"});
        }

        if (!std::in_range<std::uint16_t>(*port))
        {
            return std::unexpected(
                ConfigError{.code = ConfigErrorCode::value_out_of_range,
                            .config_path = config_path,
                            .key = "tcp_server.port",
                            .diagnostic = "Configuration value tcp_server.port must be an integer from 0 to 65535"});
        }

        if (*max_backlog < 0 || *max_backlog > std::numeric_limits<int>::max())
        {
            return std::unexpected(
                ConfigError{.code = ConfigErrorCode::value_out_of_range,
                            .config_path = config_path,
                            .key = "tcp_server.max_backlog",
                            .diagnostic = "Configuration value tcp_server.max_backlog must be a non-negative integer"});
        }

        m_config.tcp_server = {.port = static_cast<std::uint16_t>(*port),
                               .max_backlog = static_cast<int>(*max_backlog)};
    }
    catch (const std::exception &exception)
    {
        return std::unexpected(ConfigError{.code = ConfigErrorCode::parse_failure,
                                           .config_path = config_path,
                                           .key = "",
                                           .diagnostic = exception.what()});
    }

    m_is_loaded = true;
    return {};
}