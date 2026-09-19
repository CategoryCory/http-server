find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

function(add_http_server_test target source)
    add_executable(${target} ${source})

    target_compile_features(${target} PRIVATE cxx_std_23)

    target_link_libraries(
        ${target}
        PRIVATE
        GTest::gtest_main
        http_server::lib
    )

    gtest_discover_tests(${target} WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
endfunction()

add_http_server_test(http_server_config_loader_tests http_server_config_loader_test.cpp)
add_http_server_test(http_server_tests tcp_server_test.cpp)
add_http_server_test(tcp_socket_tests tcp_socket_test.cpp)
add_http_server_test(unique_file_descriptor_tests unique_file_descriptor_test.cpp)