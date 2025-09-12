
#include <benchmark/benchmark.h>
#include "iris/logger/logger.h"

const int NUM_OPS = 100'000;

static void BM_log_disabled_100k (benchmark::State& state) {
    for (auto _ : state) {
        for (int i = 0; i < NUM_OPS; i ++) {
            IRIS_DEBUG(iris::logger::rootLogger, "The format", IRIS_ARGUMENT("label", 0.5));
        }
    }
}
BENCHMARK(BM_log_disabled_100k);

static void BM_log_no_arguments_100k (benchmark::State& state) {
    for (auto _ : state) {
        for (int i = 0; i < NUM_OPS; i ++) {
            IRIS_FATAL(
                iris::logger::rootLogger,
                "The format");
        }
    }
}
BENCHMARK(BM_log_no_arguments_100k);

static void BM_log_two_arguments_100k (benchmark::State& state) {
    for (auto _ : state) {
        for (int i = 0; i < NUM_OPS; i ++) {
            IRIS_FATAL(
                iris::logger::rootLogger,
                "The format",
                IRIS_ARGUMENT("label1", 1),
                IRIS_ARGUMENT("label2", 0.5));
        }
    }
}
BENCHMARK(BM_log_two_arguments_100k);

BENCHMARK_MAIN();
