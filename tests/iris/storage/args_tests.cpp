
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
#include "iris/storage/args.h"

#include "utils/fsverify.h"

using namespace iris::storage::args;
using namespace iris::storage::object;
using namespace iris::storage::type;

TEST(ArgsStorageTests, TestSDArguments) {
    for (size_t off = 0; off < 10; off ++) {
        const auto res = IRIS_MAKE_ARGUMENTS(IRIS_ARGUMENT("abc", 1), IRIS_ARGUMENT("def", static_cast<float>(2.0)));
        EXPECT_EQ(res.first,  0);
        EXPECT_EQ(res.second, off);
    }

    int typesFd = iris::fs::rfsPolicy.open(TYPE_STORAGE_LOCATION);
    VERIFY_FD( typesFd, static_cast<TypeIntType>(0), prim_mark, Primitive::FLOAT);
    VERIFY_FD( typesFd, static_cast<TypeIntType>(1), prim_mark, Primitive::INT);
    VERIFY_EOF_FD( typesFd );

    int objectsFd = iris::fs::rfsPolicy.open(OBJECT_STORAGE_LOCATION);
    for (size_t off = 0; off < 10; off ++) {
        VERIFY_FD(
            objectsFd,
            static_cast<TypeIntType>(0),
            static_cast<ObjectIntType>(2 * off),
            static_cast<size_t>(sizeof(float)),
            static_cast<float>(2.0)
        );
        VERIFY_FD(
            objectsFd,
            static_cast<TypeIntType>(1),
            static_cast<ObjectIntType>(2 * off + 1),
            static_cast<size_t>(sizeof(int)),
            static_cast<int>(1)
        );
    }
    VERIFY_EOF_FD(objectsFd);

    int argvaluesFd = iris::fs::rfsPolicy.open(ARG_VALUES_LOCATION);
    for (size_t off = 0; off < 10; off ++) {
        VERIFY_FD(argvaluesFd, 2ULL, static_cast<ObjectIntType>(2 * off + 1), static_cast<ObjectIntType>(2 * off));
    }
    VERIFY_EOF_FD(argvaluesFd);
    
    int nameFd   = iris::fs::rfsPolicy.open(ARG_NAMES_LOCATION);
    int schemeFd = iris::fs::rfsPolicy.open(ARG_SCHEME_LOCATION);

    VERIFY_FD(nameFd, static_cast<ArgNameIntType>(0), 3ULL, 'd', 'e', 'f');
    VERIFY_FD(nameFd, static_cast<ArgNameIntType>(1), 3ULL, 'a', 'b', 'c');
    VERIFY_EOF_FD(nameFd);

    VERIFY_FD(schemeFd, 2ULL, static_cast<ArgNameIntType>(1), static_cast<ArgNameIntType>(0));
    VERIFY_EOF_FD(schemeFd);
}
