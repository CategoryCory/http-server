#pragma once

#include <config/http_server_config.hpp>

#include <http/result.hpp>

#include <filesystem>
#include <string_view>

/// @brief Loads HTTP server configuration from a TOML file.
class HttpServerConfigLoader
{
public:
    /// @brief Path to the default application configuration file.
    static constexpr std::string_view DEFAULT_CONFIG_PATH = "config/http-server.toml";

    /// @brief Loads and validates the default TOML configuration file.
    /// @return A successful result when parsing and validation succeed;
    ///         otherwise a failure result containing the error.
    [[nodiscard]] Result load();

    /// @brief Loads and validates a TOML configuration file.
    /// @param config_path Path to the TOML configuration file.
    /// @return A successful result when parsing and validation succeed;
    ///         otherwise a failure result containing the error.
    [[nodiscard]] Result load(const std::filesystem::path& config_path);

    /// @brief Checks whether configuration was successfully loaded.
    [[nodiscard]] bool is_loaded() const { return m_is_loaded; }

    /// @brief Gets the loaded application configuration.
    [[nodiscard]] const HttpServerConfig& config() const { return m_config; }

private:
    HttpServerConfig m_config{};
    bool m_is_loaded{false};
};