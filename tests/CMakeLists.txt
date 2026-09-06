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