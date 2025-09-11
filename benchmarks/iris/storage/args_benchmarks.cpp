
#include <benchmark/benchmark.h>
#include "iris/storage/args.h"

const int NUM_OPS = 100'000;

static void BM_args_static_name_static_objects_100k (benchmark::State& state) {
    for (auto _ : state) {
        for (int i = 0; i < NUM_OPS; i ++) {
            auto em = IRIS_MAKE_ARGUMENTS_SNSO(
                IRIS_ARGUMENT( "abc", 2 ),
                IRIS_ARGUMENT( "def", 2.0 )
            );
            benchmark::DoNotOptimize(em);
        }
    }
}
BENCHMARK(BM_args_static_name_static_objects_100k);

static void BM_args_static_name_dynamic_objects_100k (benchmark::State& state) {
    for (auto _ : state) {
        for (int i = 0; i < NUM_OPS; i ++) {
            auto em = IRIS_MAKE_ARGUMENTS_SNDO(
                IRIS_ARGUMENT( "abc", 2 ),
                IRIS_ARGUMENT( "def", 2.0 )
            );
            benchmark::DoNotOptimize(em);
        }
    }
}
BENCHMARK(BM_args_static_name_dynamic_objects_100k);

static void BM_args_dynamic_name_dynamic_objects_100k (benchmark::State& state) {
    for (auto _ : state) {
        for (int i = 0; i < NUM_OPS; i ++) {
            auto em = IRIS_MAKE_ARGUMENTS_DNDO(
                IRIS_ARGUMENT( "abc", 2 ),
                IRIS_ARGUMENT( "def", 2.0 )
            );
            benchmark::DoNotOptimize(em);
        }
    }
}
BENCHMARK(BM_args_dynamic_name_dynamic_objects_100k);

BENCHMARK_MAIN();
