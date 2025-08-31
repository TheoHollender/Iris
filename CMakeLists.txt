cmake_minimum_required(VERSION 3.16)
project(IrisRegistry CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(IRIS_TESTING OFF)
option(IRIS_BENCHMARK OFF)

add_library(iris)
target_sources(
    iris PRIVATE
        src/iris/core/init.cpp)
target_include_directories(iris PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

enable_testing()

if (IRIS_TESTING)
    add_subdirectory(tests)
endif()

if (IRIS_BENCHMARK)
    add_subdirectory(benchmarks)
endif()
