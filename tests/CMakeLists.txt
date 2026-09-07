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

add_executable(
    tcp_socket_tests
    tcp_socket_test.cpp
)

target_compile_features(
    tcp_socket_tests
    PRIVATE
    cxx_std_23
)

target_link_libraries(
    tcp_socket_tests
    PRIVATE
    http_server::lib
)

add_test(
    NAME tcp_socket_test
    COMMAND tcp_socket_tests
)

add_executable(
    unique_file_descriptor_tests
    unique_file_descriptor_test.cpp
)

target_compile_features(
    unique_file_descriptor_tests
    PRIVATE
    cxx_std_23
)

target_link_libraries(
    unique_file_descriptor_tests
    PRIVATE
    http_server::lib
)

add_test(
    NAME unique_file_descriptor_test
    COMMAND unique_file_descriptor_tests
)

add_executable(
    result_tests
    result_test.cpp
)

target_compile_features(
    result_tests
    PRIVATE
    cxx_std_23
)

target_link_libraries(
    result_tests
    PRIVATE
    http_server::lib
)

add_test(
    NAME result_test
    COMMAND result_tests
)