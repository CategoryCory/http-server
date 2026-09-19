add_library(
    http_server_lib
    tcp_server.cpp
    unique_file_descriptor.cpp
    tcp_socket.cpp
    http_server_config_loader.cpp
)

add_library(
    http_server::lib
    ALIAS http_server_lib
)

find_package(tomlplusplus CONFIG REQUIRED)

target_include_directories(
    http_server_lib
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

target_compile_features(
    http_server_lib
    PUBLIC
    cxx_std_23
)

target_link_libraries(
    http_server_lib
    PUBLIC
    tomlplusplus::tomlplusplus
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
        http_server_lib
        PUBLIC
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

add_executable(
    http_server
    main.cpp
)

target_compile_features(
    http_server
    PRIVATE
    cxx_std_23
)

target_link_libraries(
    http_server
    PRIVATE
    http_server::lib
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
        http_server
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()