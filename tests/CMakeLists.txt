include(FetchContent)
FetchContent_Declare(
    googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
FetchContent_MakeAvailable(googletest)

add_executable(registry_tests iris/core/registry_tests.cpp)
target_link_libraries(registry_tests PRIVATE iris gtest_main)

add_executable(mem_policy_tests iris/fs/contrib/mem_policy_tests.cpp)
target_link_libraries(mem_policy_tests PRIVATE iris_testing gtest_main)

include(GoogleTest)
gtest_discover_tests(registry_tests)
gtest_discover_tests(mem_policy_tests)