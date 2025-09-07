#include "gtest/gtest.h"
#include "iris/core/registry.h"

#include <utility>
#include <functional>

/** Test that the static macro works as expected */
TEST(RegistryTests, StaticMacroSimpleTest) {
    iris::core::Registry<int, int> my_registry;
    
    int id1 = IRIS_USE_STATIC_FROM_REGISTRY(my_registry, 1);
    EXPECT_EQ(id1, 0);
    int id2 = IRIS_USE_STATIC_FROM_REGISTRY(my_registry, 2);
    EXPECT_EQ(id2, 1);
    int id3 = IRIS_USE_STATIC_FROM_REGISTRY(my_registry, 1);
    EXPECT_EQ(id3, 0);    
    int id4 = IRIS_USE_STATIC_FROM_REGISTRY(my_registry, 2);
    EXPECT_EQ(id4, 1);
}
/** Test that the dynamic macro works as expected */
TEST(RegistryTests, DynamicMacroSimpleTest) {
    iris::core::Registry<int, int> my_registry;

    std::vector<int> values = { 5, 2, 3, 1, 4, 0 };
    std::vector<int> inv_values(values.size());
    for (int i = 0; i < values.size(); i ++) {
        int id = IRIS_USE_FROM_REGISTRY(my_registry, values[i]);
    
        EXPECT_EQ(id, i);
        inv_values[values[i]] = i;
    }

    for (int i = 0; i < values.size(); i ++) {
        int id = IRIS_USE_FROM_REGISTRY(my_registry, i);
        EXPECT_EQ(id, inv_values[i]);
    }
}
/** Test that the dynamic and static macro work together */
TEST(RegistryTests, StaticAndDynamicTest) {
    iris::core::Registry<int, int> my_registry;

    IRIS_USE_STATIC_FROM_REGISTRY(my_registry, 2);
    IRIS_USE_STATIC_FROM_REGISTRY(my_registry, 0);
    IRIS_USE_STATIC_FROM_REGISTRY(my_registry, 1);
    std::vector<int> inv_values = { 1, 2, 0 };
    for (int i = 0; i < inv_values.size(); i ++) {
        int id = IRIS_USE_FROM_REGISTRY(my_registry, i);
        EXPECT_EQ(id, inv_values[i]);
    }

    std::vector<int> values = { 3, 5, 4 };
    for (int i = 0; i < values.size(); i ++)
        IRIS_USE_FROM_REGISTRY(my_registry, values[i]);
    
    EXPECT_EQ(IRIS_USE_STATIC_FROM_REGISTRY(my_registry, 3), 3);
    EXPECT_EQ(IRIS_USE_STATIC_FROM_REGISTRY(my_registry, 4), 5);
    EXPECT_EQ(IRIS_USE_STATIC_FROM_REGISTRY(my_registry, 5), 4);
}

std::vector<std::pair<int, int>> calls;
void call_on_add (const int &key, int val) {
    calls.push_back({ key, val });
}

/** Test that the registry properly calls the passed callback function */
TEST(RegistryTests, OnAddCallbackTest) {
    iris::core::Registry<int, int> reg1(&call_on_add);

    std::function<void(const int&, int)> call_on_add_func(&call_on_add);
    iris::core::Registry<int, int> reg2(call_on_add_func);

    reg1.get(1);
    reg1.get(2);
    reg2.get(3);
    reg2.get(4);

    EXPECT_EQ(calls.size(), 4);
    EXPECT_EQ(calls[0], (std::make_pair<int, int>( 1, 0 )));
    EXPECT_EQ(calls[1], (std::make_pair<int, int>( 2, 1 )));
    EXPECT_EQ(calls[2], (std::make_pair<int, int>( 3, 0 )));
    EXPECT_EQ(calls[3], (std::make_pair<int, int>( 4, 1 )));
}
