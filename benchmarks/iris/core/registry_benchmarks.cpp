
#include <benchmark/benchmark.h>
#include <iris/core/registry.h>

const int NUM_OPS = 100'000;

const int NUM_SAMPLES_1 = 20;
const int NUM_SAMPLES_2 = 1000;

static void BM_static_int_registry_100k (benchmark::State& state) {
    iris::core::Registry<int, int> my_registry;
    for (int i = 0; i < NUM_SAMPLES_2; i ++)
        my_registry.get(i);

    for (auto _ : state) {
        for (int i = 0; i < NUM_OPS; i ++) {
            int em = IRIS_USE_STATIC_FROM_REGISTRY(my_registry, 42);
            benchmark::DoNotOptimize(em);
        }
    }
}
BENCHMARK(BM_static_int_registry_100k);

static void BM_int_registry_100k_20s (benchmark::State& state) {
    iris::core::Registry<int, int> my_registry;
    for (int i = 0; i < NUM_SAMPLES_1; i ++)
        my_registry.get(i);

    for (auto _ : state) {
        for (int i = 0; i < NUM_OPS; i ++) {
            int em = IRIS_USE_FROM_REGISTRY(my_registry, 12);
            benchmark::DoNotOptimize(em);
        }
    }
}
BENCHMARK(BM_int_registry_100k_20s);

static void BM_int_registry_100k_1000s (benchmark::State& state) {
    iris::core::Registry<int, int> my_registry;
    for (int i = 0; i < NUM_SAMPLES_2; i ++)
        my_registry.get(i);

    for (auto _ : state) {
        for (int i = 0; i < NUM_OPS; i ++) {
            int em = IRIS_USE_FROM_REGISTRY(my_registry, 12);
            benchmark::DoNotOptimize(em);
        }
    }
}
BENCHMARK(BM_int_registry_100k_1000s);

BENCHMARK_MAIN();
