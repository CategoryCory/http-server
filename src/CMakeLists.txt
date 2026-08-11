add_executable(
    http_server
    main.cpp
)

target_compile_features(
    http_server
    PRIVATE
    cxx_std_23
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