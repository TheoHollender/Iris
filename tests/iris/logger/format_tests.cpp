
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
#include "iris/logger/format.h"

TEST(FormatRegistryTests, TestFormatIds) {
    uint64_t f1 = IRIS_LOGGER_MAKE_FORMAT("Hello: '%1'");
    uint64_t f2 = IRIS_LOGGER_MAKE_FORMAT("Hello: '%2'");
    uint64_t f3 = IRIS_LOGGER_MAKE_FORMAT("Hello: '%1'");
    uint64_t f4 = IRIS_LOGGER_MAKE_FORMAT("Hello: '%2'");
    EXPECT_EQ(f1, f3);
    EXPECT_EQ(f1, 0);
    EXPECT_EQ(f2, f4);
    EXPECT_EQ(f2, 1);
}
TEST(FormatRegistryTests, TestWrittenData) {
    uint64_t f1 = IRIS_LOGGER_MAKE_FORMAT("Hello: '%1'");
    uint64_t f2 = IRIS_LOGGER_MAKE_FORMAT("Hi: '%2'");
    uint64_t f3 = IRIS_LOGGER_MAKE_FORMAT("Hello: '%1'");
    uint64_t f4 = IRIS_LOGGER_MAKE_FORMAT("Hi: '%2'");
    
    EXPECT_STREQ(FORMAT_STORAGE_LOCATION, "logs/formats");
    int fd = iris::fs::rfsPolicy.open("logs/formats");

    uint8_t ptr[8 + 8 + 11 + 8 + 8 + 8 + 1];
    EXPECT_EQ(
        iris::fs::rfsPolicy.read(fd, ptr, 8 + 8 + 11 + 8 + 8 + 8 + 1),
        8 + 8 + 11 + 8 + 8 + 8
    );

    EXPECT_EQ(*((uint64_t*) (ptr)), 0);
    EXPECT_EQ(*((uint64_t*) (ptr + 8)), 11);
    EXPECT_EQ(*((uint64_t*) (ptr + 27)), 1);
    EXPECT_EQ(*((uint64_t*) (ptr + 35)), 8);
    ptr[27] = 0;
    ptr[51] = 0;
    EXPECT_STREQ(((char*) (ptr + 16)), "Hello: '%1'");
    EXPECT_STREQ(((char*) (ptr + 43)), "Hi: '%2'");
}
