include(FetchContent)
FetchContent_Declare(
    googlebenchmark
        URL https://github.com/google/benchmark/archive/refs/tags/v1.9.4.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
set(BENCHMARK_ENABLE_TESTING OFF)
FetchContent_MakeAvailable(googlebenchmark)

function(add_benchmark benchmark_name benchmark_executable)
    if(IRIS_BENCHMARK AND NOT IRIS_TESTING)
        message(STATUS "Adding benchmark '${benchmark_name}' to ctest.")
        add_test(NAME ${benchmark_name} COMMAND ${benchmark_executable})
    endif()
endfunction()

add_executable(registry_benchmarks iris/core/registry_benchmarks.cpp)
target_link_libraries( registry_benchmarks PRIVATE iris benchmark )
add_benchmark(registry_benchmarks registry_benchmarks)
