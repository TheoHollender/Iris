
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
#include "iris/storage/types.h"

#include "utils/fsverify.h"

#define TEST_PRIMITIVE(T) { \
    int i = IRIS_MAKE_TYPE_UUID(T); \
    int j = IRIS_MAKE_TYPE_UUID(const T); \
    EXPECT_EQ(i, j); EXPECT_EQ(i, offset); \
    offset ++; \
}

using namespace iris::storage::type;

TEST(TypeStorageTests, RegisterPrimitiveTests) {
    int i1 = IRIS_MAKE_TYPE_UUID(int);
    EXPECT_EQ(i1, 0);
    int i2 = IRIS_MAKE_TYPE_UUID(const int);
    EXPECT_EQ(i2, 0);
    int i3 = IRIS_MAKE_TYPE_UUID(unsigned int);
    EXPECT_EQ(i3, 1);
    int i4 = IRIS_MAKE_TYPE_UUID(const unsigned int);
    EXPECT_EQ(i4, 1);

    int offset = 2;
    TEST_PRIMITIVE(char);
    TEST_PRIMITIVE(short);
    TEST_PRIMITIVE(long);
    TEST_PRIMITIVE(long long);
    
    TEST_PRIMITIVE(unsigned char);
    TEST_PRIMITIVE(unsigned short);
    TEST_PRIMITIVE(unsigned long);
    TEST_PRIMITIVE(unsigned long long);

    TEST_PRIMITIVE(float);
    TEST_PRIMITIVE(double);
    TEST_PRIMITIVE(bool);

    int fd = iris::fs::rfsPolicy.open(0, TYPE_STORAGE_LOCATION);
    EXPECT_EQ(TYPE_STORAGE_LOCATION, "objects/types");

    VERIFY_FD(fd, static_cast<TypeIntType>(0), prim_mark, Primitive::INT);
    VERIFY_FD(fd, static_cast<TypeIntType>(1), prim_mark, Primitive::UINT);
    
    VERIFY_FD(fd, static_cast<TypeIntType>(2), prim_mark, Primitive::CHAR);
    VERIFY_FD(fd, static_cast<TypeIntType>(3), prim_mark, Primitive::SHORT);
    VERIFY_FD(fd, static_cast<TypeIntType>(4), prim_mark, Primitive::LONG);
    VERIFY_FD(fd, static_cast<TypeIntType>(5), prim_mark, Primitive::LL);
    
    VERIFY_FD(fd, static_cast<TypeIntType>(6), prim_mark, Primitive::UCHAR);
    VERIFY_FD(fd, static_cast<TypeIntType>(7), prim_mark, Primitive::USHORT);
    VERIFY_FD(fd, static_cast<TypeIntType>(8), prim_mark, Primitive::ULONG);
    VERIFY_FD(fd, static_cast<TypeIntType>(9), prim_mark, Primitive::ULL);
    
    VERIFY_FD(fd, static_cast<TypeIntType>(10), prim_mark, Primitive::FLOAT);
    VERIFY_FD(fd, static_cast<TypeIntType>(11), prim_mark, Primitive::DOUBLE);
    VERIFY_FD(fd, static_cast<TypeIntType>(12), prim_mark, Primitive::BOOL);
    VERIFY_EOF_FD(fd);
}

struct A {
    bool x;
    double* y;
};
struct B {
    short x;
    A y;
    unsigned char z;
};

template<>
std::vector<field> iris::storage::type::fields_of_type<A> () {
    return {
        IRIS_FIELD(A, bool, x),
        IRIS_PTR_FIELD(A, y)
    };
}
template<>
std::vector<field> iris::storage::type::fields_of_type<B> () {
    return {
        IRIS_FIELD(B, short, x),
        IRIS_FIELD(B, A, y),
        IRIS_FIELD(B, unsigned char, z)
    };
}

TEST(TypeStorageTests, RegisterSampleStructsIds) {
    EXPECT_EQ(IRIS_MAKE_TYPE_UUID(B), 0);
    EXPECT_EQ(IRIS_MAKE_TYPE_UUID(short), 1);
    EXPECT_EQ(IRIS_MAKE_TYPE_UUID(A), 2);
    EXPECT_EQ(IRIS_MAKE_TYPE_UUID(bool), 3);
    EXPECT_EQ(IRIS_MAKE_TYPE_UUID(unsigned char), 4);
}
TEST(TypeStorageTests, RegisterSampleStructs) {
    EXPECT_EQ(IRIS_MAKE_TYPE_UUID(B), 0);

    int fd = iris::fs::rfsPolicy.open(0, TYPE_STORAGE_LOCATION);
    EXPECT_EQ(TYPE_STORAGE_LOCATION, "objects/types");
    
    VERIFY_FD( fd, static_cast<TypeIntType>(1), prim_mark, Primitive::SHORT);
    VERIFY_FD( fd, static_cast<TypeIntType>(3), prim_mark, Primitive::BOOL);
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(2),
        static_cast<size_t>(sizeof(A)),
        static_cast<size_t>(2),
        static_cast<TypeIntType>(3), static_cast<size_t>(offsetof(A, x)),
        POINTER_TYPE_ID, static_cast<size_t>(offsetof(A, y))
    );
    VERIFY_FD( fd, static_cast<TypeIntType>(4), prim_mark, Primitive::UCHAR);
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(0),
        static_cast<size_t>(sizeof(B)),
        static_cast<size_t>(3),
        static_cast<TypeIntType>(1), static_cast<size_t>(offsetof(B, x)),
        static_cast<TypeIntType>(2), static_cast<size_t>(offsetof(B, y)),
        static_cast<TypeIntType>(4), static_cast<size_t>(offsetof(B, z))
    );
    VERIFY_EOF_FD(fd);
}
