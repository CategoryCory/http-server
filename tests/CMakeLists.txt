add_executable(
    http_server_tests
    smoke_test.cpp
)

target_compile_features(
    http_server_tests
    PRIVATE
    cxx_std_23
)

add_test(
    NAME smoke_test
    COMMAND http_server_tests
)