
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

#include "utils/mkbuffer.h"
#include "iris/fs/policy.h"
#include "gtest/gtest.h"
#include <iostream>

#define VERIFY_FD(fd, ...) ([&]() { \
    auto buffer = create_buffer(__VA_ARGS__); \
 \
    uint8_t ptr[buffer.size()]; \
    size_t bytes_read = iris::fs::rfsPolicy.read(fd, ptr, buffer.size()); \
 \
    EXPECT_EQ(bytes_read, buffer.size()); \
    bool valid = true; \
    for (size_t offset = 0; offset < buffer.size(); offset ++) { \
        EXPECT_EQ(ptr[offset], buffer[offset]); \
        if (ptr[offset] != buffer[offset]) valid = false; \
    } \
    if (!valid) { \
      std::cout << "buffer: "; \
      for (size_t offset = 0; offset < buffer.size(); offset ++) \
         std::cout << ((unsigned int) buffer[offset]) << " "; \
      std::cout << std::endl; \
      std::cout << "ptr: "; \
      for (size_t offset = 0; offset < bytes_read; offset ++) \
         std::cout << ((unsigned int) ptr[offset]) << " "; \
      std::cout << std::endl; \
    } \
})();
#define VERIFY_EOF_FD(fd) ([&]() { \
    uint8_t ptr[1]; \
    EXPECT_EQ(0, iris::fs::rfsPolicy.read(fd, ptr, 1)); \
})();
