cmake_minimum_required(VERSION 3.16)
project(IrisRegistry CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_library(iris)
target_sources(
    iris PRIVATE
        src/iris/core/init.cpp)
target_include_directories(iris PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(tests)
add_subdirectory(benchmarks)
