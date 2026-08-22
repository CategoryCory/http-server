add_executable(
    http_server_tests
    tcp_server_test.cpp
)

target_compile_features(
    http_server_tests
    PRIVATE
    cxx_std_23
)

target_link_libraries(
    http_server_tests
    PRIVATE
    http_server::lib
)

add_test(
    NAME tcp_server_test
    COMMAND http_server_tests
)