
#include <benchmark/benchmark.h>
#include "iris/fs/policy.h"

using namespace iris::fs;

/**
 * Base implementation of wfs and rfs policies.
 */

uint32_t WriteFileSystemPolicy::open (std::string path) {
    benchmark::DoNotOptimize(path);
    return 0;
}
void WriteFileSystemPolicy::write (uint32_t fd, const uint8_t* ptr, uint32_t size) {
    benchmark::DoNotOptimize(fd);
    benchmark::DoNotOptimize(ptr);
    benchmark::DoNotOptimize(size);
}
// LCOV_EXCL_START
void WriteFileSystemPolicy::flush (uint32_t fd) {
    benchmark::DoNotOptimize(fd);
}
void WriteFileSystemPolicy::flush_all () {}
// LCOV_EXCL_STOP

uint32_t ReadFileSystemPolicy::open (std::string path) {
    benchmark::DoNotOptimize(path);
    return 0;
}
uint32_t ReadFileSystemPolicy::read (uint32_t fd, uint8_t* ptr, uint32_t size) {
    benchmark::DoNotOptimize(fd);
    benchmark::DoNotOptimize(ptr);
    benchmark::DoNotOptimize(size);
    return 0;
}

WriteFileSystemPolicy iris::fs::wfsPolicy = WriteFileSystemPolicy();
ReadFileSystemPolicy  iris::fs::rfsPolicy = ReadFileSystemPolicy();
