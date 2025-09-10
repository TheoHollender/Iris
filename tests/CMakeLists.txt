include(FetchContent)
FetchContent_Declare(
    googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
FetchContent_MakeAvailable(googletest)

function (setup_link test_name)
    target_link_libraries(${test_name} PRIVATE iris_testing gtest_main)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${test_name} PRIVATE
            -fprofile-arcs
            -ftest-coverage
        )
        target_link_options(${test_name} PRIVATE
            -fprofile-arcs
            -ftest-coverage
        )
    endif()
endfunction()

add_executable(registry_tests iris/core/registry_tests.cpp)
setup_link(registry_tests)

add_executable(mem_policy_tests iris/fs/contrib/mem_policy_tests.cpp)
setup_link(mem_policy_tests)

add_executable(format_tests iris/logger/format_tests.cpp)
setup_link(format_tests)

add_executable(array_tests iris/storage/array_tests.cpp)
setup_link(array_tests)

add_executable(argscheme_tests iris/storage/argscheme_tests.cpp)
target_include_directories(argscheme_tests PUBLIC .)
setup_link(argscheme_tests)

add_executable(types_tests iris/storage/types_tests.cpp)
target_include_directories(types_tests PUBLIC .)
setup_link(types_tests)

include(GoogleTest)
gtest_discover_tests(registry_tests)
gtest_discover_tests(mem_policy_tests)
gtest_discover_tests(format_tests)
gtest_discover_tests(array_tests)
gtest_discover_tests(argscheme_tests)
gtest_discover_tests(types_tests)
