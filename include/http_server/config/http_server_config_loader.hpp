#pragma once

#include <http_server/config/config_error.hpp>
#include <http_server/config/http_server_config.hpp>

#include <expected>
#include <filesystem>
#include <string_view>

namespace http_server::config
{

/// @brief Loads HTTP server configuration from a TOML file.
class HttpServerConfigLoader
{
public:
    /// @brief The path to the default application configuration file.
    static constexpr std::string_view DEFAULT_CONFIG_PATH = "config/http-server.toml";

    /// @brief Loads and validates the TOML configuration file at DEFAULT_CONFIG_PATH.
    ///
    /// Equivalent to calling load(const std::filesystem::path &) with DEFAULT_CONFIG_PATH.
    /// @return An engaged std::expected when parsing and validation succeed;
    ///         otherwise an unexpected ConfigError.
    [[nodiscard]] std::expected<void, ConfigError> load();

    /// @brief Loads and validates a TOML configuration file.
    ///
    /// Any previously loaded configuration is discarded first, so after a failure the loader holds
    /// default-initialized values and is_loaded() returns false.
    /// @param config_path Path to the TOML configuration file.
    /// @return An engaged std::expected when parsing and validation succeed;
    ///         otherwise an unexpected ConfigError.
    [[nodiscard]] std::expected<void, ConfigError> load(const std::filesystem::path &config_path);

    /// @brief Checks whether a configuration was successfully loaded.
    /// @return true when the last load succeeded; otherwise false.
    [[nodiscard]] bool is_loaded() const { return m_is_loaded; }

    /// @brief Returns the loaded application configuration.
    /// @return The loaded configuration, or default-initialized values if none has been loaded successfully.
    [[nodiscard]] const HttpServerConfig &config() const { return m_config; }

private:
    HttpServerConfig m_config{};
    bool m_is_loaded{false};
};

} // namespace http_server::config
