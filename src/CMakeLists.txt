# Defines the library for one layer as http_server_<layer> with the alias
# http_server::<layer>. Layers declare only the layers below them as dependencies.
function(add_http_server_layer layer)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "SOURCES;LINK_LIBRARIES")

    set(target http_server_${layer})

    add_library(${target} ${ARG_SOURCES})
    add_library(http_server::${layer} ALIAS ${target})

    target_include_directories(${target} PUBLIC "${PROJECT_SOURCE_DIR}/include")
    target_compile_features(${target} PUBLIC cxx_std_23)

    if(ARG_LINK_LIBRARIES)
        target_link_libraries(${target} PUBLIC ${ARG_LINK_LIBRARIES})
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PUBLIC -Wall -Wextra -Wpedantic)
    endif()
endfunction()

add_subdirectory(core)
add_subdirectory(tcp)
add_subdirectory(config)

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
    http_server::config
    http_server::tcp
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
