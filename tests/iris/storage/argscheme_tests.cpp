
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
#include "iris/storage/argscheme.h"

#include "utils/fsverify.h"

using namespace iris::storage::args;

TEST(ArgSchemeStorageTests, TestStorageSingleElement) {
    EXPECT_EQ( IRIS_MAKE_ARGSCHEME("name"), 0 );
    EXPECT_EQ( IRIS_MAKE_ARGSCHEME("name"), 1 );

    int fd_name = iris::fs::rfsPolicy.open(0, ARG_NAMES_LOCATION);
    int fd_schm = iris::fs::rfsPolicy.open(0, ARG_SCHEME_LOCATION);

    VERIFY_FD( fd_name, static_cast<ArgNameIntType>(0), 4ULL, 'n', 'a', 'm', 'e' );
    VERIFY_EOF_FD( fd_name );

    VERIFY_FD( fd_schm, 1ULL, static_cast<ArgNameIntType>(0) );
    VERIFY_FD( fd_schm, 1ULL, static_cast<ArgNameIntType>(0) );
    VERIFY_EOF_FD( fd_schm );
}

TEST(ArgSchemeStorageTests, TestStorageMultipleElement) {
    EXPECT_EQ( IRIS_MAKE_ARGSCHEME("label1", "label2", "label3"), 0 );
    EXPECT_EQ( IRIS_MAKE_ARGSCHEME("label3", "label2", "label1", "label3"), 1 );

    uint8_t label1 = IRIS_MAKE_ARGNAME("label1");
    uint8_t label2 = IRIS_MAKE_ARGNAME("label2");
    uint8_t label3 = IRIS_MAKE_ARGNAME("label3");
    
    int fd_name = iris::fs::rfsPolicy.open(0, ARG_NAMES_LOCATION);
    int fd_schm = iris::fs::rfsPolicy.open(0, ARG_SCHEME_LOCATION);
    
    VERIFY_FD(
        fd_schm, 3ULL,
        static_cast<ArgNameIntType>(label1),
        static_cast<ArgNameIntType>(label2),
        static_cast<ArgNameIntType>(label3)
    );
    VERIFY_FD(
        fd_schm, 4ULL,
        static_cast<ArgNameIntType>(label3),
        static_cast<ArgNameIntType>(label2),
        static_cast<ArgNameIntType>(label1),
        static_cast<ArgNameIntType>(label3)
    );
    VERIFY_EOF_FD(fd_schm);
}