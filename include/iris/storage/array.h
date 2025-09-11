
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
#include "iris/fs/policy.h"

#include <array>
#include <mutex>
#include <cstring>
#include <type_traits>

namespace iris::storage::array {
    template<typename Content, typename IntType>
    struct ArrayStorage {
    public:
        ArrayStorage (const char* storage_loc) : storage_loc(storage_loc) {}
        
        template<typename... Args>
        constexpr IntType store (Args... args)
            requires (std::is_convertible_v<Content, Args> && ...) {
            return store_same_v(static_cast<Content>(args)...);
        }
    private:
        int fd = -1;
        const char* storage_loc;
        bool pending_init = true;
        IntType num_written = 0;

        std::mutex init_mutex;
        void init () {
            if (pending_init) {
                std::lock_guard<std::mutex> lock_init(init_mutex);
                if (!pending_init) return ;

                fd = iris::fs::wfsPolicy.open(storage_loc);

                pending_init = false;
            }
        }

        template<typename... Args>
        constexpr IntType store_same_v (Args... args)
            requires (std::is_same_v<Content, Args> && ...) {
            
            constexpr size_t number_args = sizeof...(args);
            constexpr size_t header_size = sizeof(size_t);
            constexpr size_t size_buffer = header_size + sizeof(Content) * number_args;

            std::array<uint8_t, size_buffer> data_buffer;
            *((size_t*) data_buffer.data()) = number_args; // write header

            size_t offset = sizeof(size_t);

            ( 
                ..., ( 
                    memcpy(data_buffer.data() + offset, &args, sizeof(Content)),
                    offset += sizeof(Content)
                )
            );

            init();
            iris::fs::wfsPolicy.write(fd, data_buffer.data(), size_buffer);

            return num_written ++;
        }
    };
}
