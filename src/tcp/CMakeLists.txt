add_http_server_layer(
    tcp
    SOURCES
    tcp_connection.cpp
    tcp_server.cpp
    tcp_socket.cpp
    LINK_LIBRARIES
    http_server::core
)

target_compile_definitions(
    http_server_tcp
    PRIVATE
    $<$<PLATFORM_ID:Darwin>:HTTP_SERVER_PLATFORM_MACOS>
    $<$<PLATFORM_ID:Linux>:HTTP_SERVER_PLATFORM_LINUX>
)
