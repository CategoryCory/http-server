find_package(tomlplusplus CONFIG REQUIRED)

add_http_server_layer(
    config
    SOURCES
    http_server_config_loader.cpp
    LINK_LIBRARIES
    http_server::tcp
    tomlplusplus::tomlplusplus
)
