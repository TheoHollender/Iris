cmake_minimum_required(VERSION 3.16)
project(IrisRegistry CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(IRIS_TESTING OFF)
option(IRIS_BENCHMARK OFF)

add_library(iris)
target_sources(
    iris PRIVATE
        src/iris/core/init.cpp
        src/iris/logger/format.cpp
        src/iris/storage/argscheme.cpp)
target_include_directories(iris PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

enable_testing()

add_library(iris_fs_mem)
target_sources(
    iris_fs_mem PRIVATE
        src/iris/fs/contrib/mem_policy.cpp)
target_include_directories(iris_fs_mem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (IRIS_TESTING)
    add_library(iris_testing)
    target_include_directories(iris_testing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_sources(
        iris_testing PRIVATE
            src/iris/fs/contrib/mem_policy.cpp)
    target_compile_definitions(
        iris_testing PRIVATE IRIS_FS_MEM_TESTING)
        
    target_link_libraries(iris_testing PRIVATE iris)

    add_subdirectory(tests)
endif()

if (IRIS_BENCHMARK)
    add_subdirectory(benchmarks)
endif()
