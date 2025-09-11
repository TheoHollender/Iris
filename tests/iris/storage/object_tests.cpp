
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
#include "iris/storage/object.h"

#include "utils/fsverify.h"

using namespace iris::storage::object;

TEST(ObjectStorageTests, StorePrimitiveTests) {
    EXPECT_EQ( 0, IRIS_STORE_OBJECT(static_cast<char>(12)) );
    EXPECT_EQ( 1, IRIS_STORE_OBJECT(static_cast<short>(1600)) );
    EXPECT_EQ( 2, IRIS_STORE_OBJECT(static_cast<int>(2'100'300'040)) );
    EXPECT_EQ( 3, IRIS_STORE_OBJECT(static_cast<long>(1'200'400'050)) );
    EXPECT_EQ( 4, IRIS_STORE_OBJECT(static_cast<long long>(5'300'400'122'100'300'040)) );

    EXPECT_EQ( 5, IRIS_STORE_OBJECT(static_cast<unsigned char>(129)) );
    EXPECT_EQ( 6, IRIS_STORE_OBJECT(static_cast<unsigned short>(65000)) );
    EXPECT_EQ( 7, IRIS_STORE_OBJECT(static_cast<unsigned int>(4'100'300'040)) );
    EXPECT_EQ( 8, IRIS_STORE_OBJECT(static_cast<unsigned long>(3'200'400'050)) );
    EXPECT_EQ( 9, IRIS_STORE_OBJECT(static_cast<unsigned long long>(10'300'400'122'100'300'040ULL)) );
    
    EXPECT_EQ( 10, IRIS_STORE_OBJECT(static_cast<float>(31.75632)) );
    EXPECT_EQ( 11, IRIS_STORE_OBJECT(static_cast<double>(65.265546452236)) );
    EXPECT_EQ( 12, IRIS_STORE_OBJECT(static_cast<bool>(true)) );

    int fd = iris::fs::rfsPolicy.open(OBJECT_STORAGE_LOCATION);
    EXPECT_EQ(OBJECT_STORAGE_LOCATION, "objects/instances");

    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(0), static_cast<ObjectIntType>(0),
        static_cast<size_t>(sizeof(char)), static_cast<char>(12) );
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(1), static_cast<ObjectIntType>(1),
        static_cast<size_t>(sizeof(short)), static_cast<short>(1600) );
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(2), static_cast<ObjectIntType>(2),
        static_cast<size_t>(sizeof(int)), static_cast<int>(2'100'300'040) );
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(3), static_cast<ObjectIntType>(3),
        static_cast<size_t>(sizeof(long)), static_cast<long>(1'200'400'050) );
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(4), static_cast<ObjectIntType>(4),
        static_cast<size_t>(sizeof(long long)), static_cast<long long>(5'300'400'122'100'300'040) );
    
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(5), static_cast<ObjectIntType>(5),
        static_cast<size_t>(sizeof(unsigned char)), static_cast<unsigned char>(129) );
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(6), static_cast<ObjectIntType>(6),
        static_cast<size_t>(sizeof(unsigned short)), static_cast<unsigned short>(65000) );
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(7), static_cast<ObjectIntType>(7),
        static_cast<size_t>(sizeof(unsigned int)), static_cast<unsigned int>(4'100'300'040) );
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(8), static_cast<ObjectIntType>(8),
        static_cast<size_t>(sizeof(unsigned long)), static_cast<unsigned long>(3'200'400'050) );
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(9), static_cast<ObjectIntType>(9),
        static_cast<size_t>(sizeof(unsigned long long)), static_cast<unsigned long long>(10'300'400'122'100'300'040ULL) );
    
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(10), static_cast<ObjectIntType>(10),
        static_cast<size_t>(sizeof(float)), static_cast<float>(31.75632) );
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(11), static_cast<ObjectIntType>(11),
        static_cast<size_t>(sizeof(double)), static_cast<double>(65.265546452236) );
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(12), static_cast<ObjectIntType>(12),
        static_cast<size_t>(sizeof(bool)), static_cast<bool>(true) );
    
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
        FIELD(A, bool, x),
        PTR_FIELD(A, y)
    };
}
template<>
std::vector<field> iris::storage::type::fields_of_type<B> () {
    return {
        FIELD(B, short, x),
        FIELD(B, A, y),
        FIELD(B, unsigned char, z)
    };
}

// LCOV_EXCL_START
template<>
void iris::storage::object::prepare_object<A> (const A& other) {
    IRIS_STORE_POINTER(other.y, 1, false);
}
template<>
void iris::storage::object::prepare_object<B> (const B& other) {
    prepare_object<A>(other.y);
}
// LCOV_EXCL_STOP

TEST(ObjectStorageTests, StoreStructAndPtr) {
    double dbl = 0.34454;
    B obj = { 100, { true, &dbl }, 100U };
    EXPECT_EQ( 1, IRIS_STORE_OBJECT(obj) );

    int fd = iris::fs::rfsPolicy.open(TYPE_STORAGE_LOCATION);
    EXPECT_EQ(TYPE_STORAGE_LOCATION, "objects/types");
    
    VERIFY_FD( fd, static_cast<TypeIntType>(0), prim_mark, Primitive::DOUBLE);
    VERIFY_FD( fd, static_cast<TypeIntType>(2), prim_mark, Primitive::SHORT);
    VERIFY_FD( fd, static_cast<TypeIntType>(4), prim_mark, Primitive::BOOL);
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(3),
        static_cast<size_t>(sizeof(A)),
        static_cast<size_t>(2),
        static_cast<TypeIntType>(4), static_cast<size_t>(offsetof(A, x)),
        POINTER_TYPE_ID, static_cast<size_t>(offsetof(A, y))
    );
    VERIFY_FD( fd, static_cast<TypeIntType>(5), prim_mark, Primitive::UCHAR);
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(1),
        static_cast<size_t>(sizeof(B)),
        static_cast<size_t>(3),
        static_cast<TypeIntType>(2), static_cast<size_t>(offsetof(B, x)),
        static_cast<TypeIntType>(3), static_cast<size_t>(offsetof(B, y)),
        static_cast<TypeIntType>(5), static_cast<size_t>(offsetof(B, z))
    );
    VERIFY_EOF_FD(fd);

    fd = iris::fs::rfsPolicy.open(OBJECT_STORAGE_LOCATION);
    
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(0), static_cast<ObjectIntType>(0),
        static_cast<size_t>(sizeof(double)), static_cast<double>(dbl) );
    
    size_t target_dbl = (size_t) ((void*) &dbl);
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(POINTER_TYPE_ID),
        static_cast<bool>(false),
        static_cast<size_t>(1),
        target_dbl,
        static_cast<ObjectIntType>(0)
    );
    VERIFY_FD(
        fd,
        static_cast<TypeIntType>(1), static_cast<ObjectIntType>(1),
        static_cast<size_t>(sizeof(B)), obj );
}
