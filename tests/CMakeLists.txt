find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

# Defines one test executable per layer. Sources are relative to the calling directory.
function(add_http_server_layer_tests layer)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "SOURCES")

    set(target ${layer}_tests)

    add_executable(${target} ${ARG_SOURCES})

    target_compile_features(${target} PRIVATE cxx_std_23)

    target_include_directories(${target} PRIVATE "${PROJECT_SOURCE_DIR}/tests")

    target_link_libraries(
        ${target}
        PRIVATE
        GTest::gtest_main
        http_server::${layer}
    )

    gtest_discover_tests(${target} WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
endfunction()

add_subdirectory(core)
add_subdirectory(tcp)
add_subdirectory(config)
