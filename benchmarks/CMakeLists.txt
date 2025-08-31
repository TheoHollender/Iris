include(FetchContent)
FetchContent_Declare(
    googlebenchmark
        URL https://github.com/google/benchmark/archive/refs/tags/v1.9.4.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
set(BENCHMARK_ENABLE_TESTING OFF)
FetchContent_MakeAvailable(googlebenchmark)

add_executable(registry_benchmarks iris/core/registry_benchmarks.cpp)
target_link_libraries( registry_benchmarks PRIVATE iris benchmark )