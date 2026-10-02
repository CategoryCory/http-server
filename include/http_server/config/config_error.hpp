#pragma once

#include <filesystem>
#include <string>

/// @brief Error codes for configuration parsing and validation failures.
enum class ConfigErrorCode
{
    /// @brief Failed to parse the configuration file.
    parse_failure,

    /// @brief A required configuration value is missing.
    missing_required_value,

    /// @brief A configuration value is invalid.
    invalid_value,

    /// @brief A configuration value is out of the allowed range.
    value_out_of_range,

    /// @brief An unknown error occurred while processing the configuration.
    unknown_error,
};

/// @brief Represents an error encountered during configuration parsing or validation.
struct ConfigError
{
    /// @brief The error code indicating the type of configuration error.
    ConfigErrorCode code;

    /// @brief The path to the configuration file associated with the error.
    std::filesystem::path config_path;

    /// @brief The specific configuration key associated with the error.
    ///        Empty for whole-file parse failures.
    std::string key;

    /// @brief A diagnostic message providing additional details about the error.
    ///        Contains a parser diagnostic or validation detail.
    std::string diagnostic;
};
