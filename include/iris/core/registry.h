
/**
 * MIT License
 * 
 * Copyright (c) Théo Hollender, 2025
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once
#include <map>
#include <functional>

namespace iris::core {

    template<typename KeyType, typename IntType>
    void do_nothing (const KeyType &key, IntType value) {}

    template<typename KeyType, typename IntType>
    struct Registry {
    private:
        std::map<KeyType, IntType> content;

        std::function<void(const KeyType&, IntType)> on_add;
    public:
        Registry () : on_add(&do_nothing<KeyType, IntType>) {};
        Registry (void (*on_add)(const KeyType&, IntType)) : on_add(on_add) {}
        Registry (std::function<void(const KeyType&, IntType)> on_add) : on_add(on_add) {}

        IntType get (const KeyType &target) {
            auto it = content.find(target);
            if (it != content.end()) return (*it).second;
        
            IntType result = content[target] = content.size();
            on_add(target, result);
            return result;
        }
    };

}

/**
 * Get an element from a registery in a static way.
 *   The target should be a constant expression.
 *   The call to the underlying map is done once statically.
 * 
 * This is good for highly intensive programs as the CPU will be able
 *   to predict correctly where the information is stored and this should
 *   be close to O(1).
 */
#define IRIS_USE_STATIC_FROM_REGISTRY(registry, target) ([](auto &reg) {    \
    static auto element(reg.get(target));                                   \
    return element;                                                         \
})(registry)

/**
 * Get an element from the registry. If the element is known in advance,
 *   you should use IRIS_USE_STATIC_FROM_REGISTRY, which will be much faster.
 */
#define IRIS_USE_FROM_REGISTRY(registry, target) (registry.get(target))
