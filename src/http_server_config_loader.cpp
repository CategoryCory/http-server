#include <config/http_server_config_loader.hpp>

#include <toml++/toml.hpp>

#include <exception>
#include <limits>
#include <utility>

Result HttpServerConfigLoader::load()
{
    return load(std::filesystem::path{DEFAULT_CONFIG_PATH});
}

Result HttpServerConfigLoader::load(const std::filesystem::path& config_path)
{
    m_config = {};
    m_is_loaded = false;

    try
    {
        const toml::table toml_config = toml::parse_file(config_path.string());
        const auto port = toml_config.at_path("tcp_server.port").value<std::int64_t>();
        const auto max_backlog = toml_config.at_path("tcp_server.max_backlog").value<std::int64_t>();

        if (!port || !std::in_range<std::uint16_t>(*port))
        {
            return Result::failure("Configuration value tcp_server.port must be an integer from 0 to 65535");
        }

        if (!max_backlog || *max_backlog < 0 || *max_backlog > std::numeric_limits<int>::max())
        {
            return Result::failure("Configuration value tcp_server.max_backlog must be a non-negative integer");
        }

        m_config.tcp_server = {
            .port = static_cast<std::uint16_t>(*port),
            .max_backlog = static_cast<int>(*max_backlog)
        };
    }
    catch(const std::exception& exception)
    {
        return Result::failure(exception.what());
    }

    m_is_loaded = true;
    return Result::success();
}