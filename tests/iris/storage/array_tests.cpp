
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

#include "gtest/gtest.h"
#include "iris/fs/policy.h"
#include "iris/storage/array.h"

using namespace iris::storage::array;

ArrayStorage<uint64_t, uint32_t> storage("loc");

TEST(ArrayStorageTests, TestStorageSingleElement) {
    EXPECT_EQ(storage.store(257ULL), 0);
    EXPECT_EQ(storage.store(1ULL), 1);

    int fd = iris::fs::rfsPolicy.open(0, "loc");
    uint8_t ptr[33];
    EXPECT_EQ(iris::fs::rfsPolicy.read(fd, ptr, 33), 32);

    uint8_t exp[32] = {
        1, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0
    };

    for (int i = 0; i < 32; i ++) EXPECT_EQ(ptr[i], exp[i]);
}
TEST(ArrayStorageTests, TestStorageManyElements) {
    uint8_t  word = 12;
    uint16_t shrt = (4 << 8) + 7;
    uint32_t norm = (3 << 24) + (15 << 16) + (10 << 8) + 255;
    uint64_t llng = (95LL << 56) + (47LL << 48) + (17LL << 40) + (126LL << 32) + (4 << 24) + (16 << 16) + (11 << 8) + 254;
    EXPECT_EQ( storage.store(word, shrt, norm, llng), 0 );
    EXPECT_EQ( storage.store(shrt, word, llng, norm), 1 );

    int fd = iris::fs::rfsPolicy.open(0, "loc");
    uint8_t ptr[81];
    EXPECT_EQ(iris::fs::rfsPolicy.read(fd, ptr, 81), 80);

    uint8_t exp[] = {
        4,   0,  0,  0, 0,   0,  0,  0,
        12,  0,  0,  0, 0,   0,  0,  0,
        7,   4,  0,  0, 0,   0,  0,  0,
        255, 10, 15, 3, 0,   0,  0,  0,
        254, 11, 16, 4, 126, 17, 47, 95,
        4,   0,  0,  0, 0,   0,  0,  0,
        7,   4,  0,  0, 0,   0,  0,  0,
        12,  0,  0,  0, 0,   0,  0,  0,
        254, 11, 16, 4, 126, 17, 47, 95,
        255, 10, 15, 3, 0,   0,  0,  0,
    };

    for (int i = 0; i < 80; i ++) EXPECT_EQ(ptr[i], exp[i]);
}