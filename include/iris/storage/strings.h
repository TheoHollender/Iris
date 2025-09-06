
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

#include <cstdint>
#include <string>
#include <string_view>
#include <mutex>
#include <iostream>

#include "iris/fs/policy.h"
#include "iris/core/registry.h"

namespace iris::storage {

    template<typename IntType>
    struct StringStorageRegistry {
    private:
        uint32_t fd = -1;
        const char* storage_loc;

        bool storage_pendint_init = true;

        iris::core::Registry<std::string, IntType> registry;

        std::mutex add_string_mutex;
        void on_add (const std::string &buffer, IntType &value) {
            std::lock_guard<std::mutex> guard_add (add_string_mutex);
            
            size_t bf_size = buffer.size();
            iris::fs::wfsPolicy.write(fd, (const uint8_t*) &value, sizeof(IntType));
            iris::fs::wfsPolicy.write(fd, (const uint8_t*) &bf_size, sizeof(size_t));
            iris::fs::wfsPolicy.write(fd, (const uint8_t*) buffer.data(), buffer.size());
        }
        void init () {
            if (storage_pendint_init) {
                fd = iris::fs::wfsPolicy.open(storage_loc);
                
                std::function<void(const std::string&, IntType)> on_add_wrapper(
                    [this](const std::string &buffer, IntType value) {
                        on_add(buffer, value);
                        return ;
                    }
                );

                registry = iris::core::Registry<std::string, IntType>( on_add_wrapper );

                storage_pendint_init = false;
            }
        }
    public:
        StringStorageRegistry (const char* storage_loc) : storage_loc(storage_loc) {}

        IntType get (const std::string &target) {
            init();

            return registry.get(target);
        }
    };

}
