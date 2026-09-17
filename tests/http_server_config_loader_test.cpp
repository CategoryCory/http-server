#include <config/http_server_config_loader.hpp>

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string_view>

namespace {
    class TemporaryConfigFile
    {
    public:
        explicit TemporaryConfigFile(std::string_view contents)
            : m_path(std::filesystem::temp_directory_path() / "http-server-config-loader-test.toml")
        {
            std::ofstream config_file{m_path};

            if (!config_file)
            {
                throw std::runtime_error("Failed to create temporary configuration file");
            }

            config_file << contents;

            if (!config_file)
            {
                throw std::runtime_error("Failed to write temporary configuration file");
            }
        }

        ~TemporaryConfigFile()
        {
            std::error_code error;
            std::filesystem::remove(m_path, error);
        }

        [[nodiscard]] const std::filesystem::path& path() const { return m_path; }

    private:
        std::filesystem::path m_path;
    };
}

TEST(HttpServerConfigLoaderTest, LoadsDefaultConfiguration)
{
    HttpServerConfigLoader config_loader;

    const Result result = config_loader.load();

    ASSERT_TRUE(result.is_success()) << result.message();
    EXPECT_TRUE(config_loader.is_loaded());
    EXPECT_EQ(config_loader.config().tcp_server.port, 8080);
    EXPECT_EQ(config_loader.config().tcp_server.max_backlog, 5);
}

TEST(HttpServerConfigLoaderTest, LoadsSpecifiedConfiguration)
{
    const TemporaryConfigFile config_file{
        "[tcp_server]\n"
        "port = 9090\n"
        "max_backlog = 10\n"
    };
    HttpServerConfigLoader config_loader;

    const Result result = config_loader.load(config_file.path());

    ASSERT_TRUE(result.is_success()) << result.message();
    EXPECT_TRUE(config_loader.is_loaded());
    EXPECT_EQ(config_loader.config().tcp_server.port, 9090);
    EXPECT_EQ(config_loader.config().tcp_server.max_backlog, 10);
}

TEST(HttpServerConfigLoaderTest, RejectsMissingRequiredValue)
{
    const TemporaryConfigFile config_file{
        "[tcp_server]\n"
        "port = 8080\n"
    };
    HttpServerConfigLoader config_loader;

    const Result result = config_loader.load(config_file.path());

    EXPECT_FALSE(result.is_success());
    EXPECT_FALSE(config_loader.is_loaded());
    EXPECT_EQ(result.message(), "Configuration value tcp_server.max_backlog must be a non-negative integer");
}

TEST(HttpServerConfigLoaderTest, RejectsOutOfRangePort)
{
    const TemporaryConfigFile config_file{
        "[tcp_server]\n"
        "port = 65536\n"
        "max_backlog = 5\n"
    };
    HttpServerConfigLoader config_loader;

    const Result result = config_loader.load(config_file.path());

    EXPECT_FALSE(result.is_success());
    EXPECT_FALSE(config_loader.is_loaded());
    EXPECT_EQ(result.message(), "Configuration value tcp_server.port must be an integer from 0 to 65535");
}

TEST(HttpServerConfigLoaderTest, RejectsMissingConfigurationFile)
{
    const std::filesystem::path missing_path =
        std::filesystem::temp_directory_path() / "http-server-missing-config.toml";
    std::error_code error;
    std::filesystem::remove(missing_path, error);
    HttpServerConfigLoader config_loader;

    const Result result = config_loader.load(missing_path);

    EXPECT_FALSE(result.is_success());
    EXPECT_FALSE(config_loader.is_loaded());
}