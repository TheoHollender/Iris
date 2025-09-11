include(FetchContent)
FetchContent_Declare(
    googlebenchmark
        URL https://github.com/google/benchmark/archive/refs/tags/v1.9.4.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
set(BENCHMARK_ENABLE_TESTING OFF)
FetchContent_MakeAvailable(googlebenchmark)

add_library(iris_fs_bm)
target_include_directories(iris_fs_bm PUBLIC ../src/include)
target_sources(
    iris_fs_bm PRIVATE
        iris/fs/contrib/bm_policy.cpp)
target_link_libraries(iris_fs_bm PRIVATE iris benchmark)
        
function(add_benchmark benchmark_name benchmark_executable)
    if(IRIS_BENCHMARK AND NOT IRIS_TESTING)
        message(STATUS "Adding benchmark '${benchmark_name}' to ctest.")
        add_test(NAME ${benchmark_name} COMMAND ${benchmark_executable})
    endif()
endfunction()

add_executable(registry_benchmarks iris/core/registry_benchmarks.cpp)
target_link_libraries( registry_benchmarks PRIVATE iris benchmark )
add_benchmark(registry_benchmarks registry_benchmarks)

add_executable(args_benchmarks iris/storage/args_benchmarks.cpp)
target_link_libraries( args_benchmarks PRIVATE iris iris_fs_bm benchmark )
add_benchmark(args_benchmarks args_benchmarks)
