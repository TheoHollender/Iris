#include "gtest/gtest.h"
#include "iris/fs/policy.h"

#include "iris/fs/contrib/mem_policy.h"
#include <iostream>

TEST(InMemoryPolicyTests, SmallTestsInternal) {
    EXPECT_EQ( iris::fs::memory::_internal::mem_storage.open_write("a.txt"), 0 );
    EXPECT_EQ( iris::fs::memory::_internal::mem_storage.open_write("b.txt"), 1 );
    EXPECT_EQ( iris::fs::memory::_internal::mem_storage.open_write("a.txt"), 0 );
    EXPECT_EQ( iris::fs::memory::_internal::mem_storage.open_write("b.txt"), 1 );

    uint8_t data[4];
    for (int i = 0; i < 4; i ++) data[i] = i + 'a';

    int fd_a = 0;
    int fd_b = 1;
    iris::fs::memory::_internal::mem_storage.write(fd_a, data, 2);
    iris::fs::memory::_internal::mem_storage.write(fd_a, data, 2);
    iris::fs::memory::_internal::mem_storage.write(fd_b, data, 4);

    EXPECT_EQ( iris::fs::memory::_internal::mem_storage.open_read("a.txt"), 0 );
    EXPECT_EQ( iris::fs::memory::_internal::mem_storage.open_read("a.txt"), 1 );
    EXPECT_EQ( iris::fs::memory::_internal::mem_storage.open_read("b.txt"), 2 );

    int rfa = 0; int rfb = 2;
    char buffer[5];
    buffer[4] = 0;

    EXPECT_EQ( iris::fs::memory::_internal::mem_storage.read( rfa, (uint8_t*) buffer, 4 ), 4 );
    EXPECT_STREQ( buffer, "abab" );
    for (int i = 0; i < 5; i ++) buffer[i] = 0;
    EXPECT_EQ( iris::fs::memory::_internal::mem_storage.read( rfb, (uint8_t*) buffer, 4 ), 4 );
    EXPECT_STREQ( buffer, "abcd" );
    for (int i = 0; i < 5; i ++) buffer[i] = 0;

    rfa = 1;
    char buffer2[7];
    for (int i = 0; i < 7; i ++) buffer2[i] = 0;

    EXPECT_EQ( iris::fs::memory::_internal::mem_storage.read( rfa, (uint8_t*) buffer2, 2 ), 2 );
    EXPECT_EQ( iris::fs::memory::_internal::mem_storage.read( rfa, (uint8_t*) (buffer2 + 2), 2 ), 2 );
    EXPECT_EQ( iris::fs::memory::_internal::mem_storage.read( rfa, (uint8_t*) (buffer2 + 4), 2 ), 0 );
    EXPECT_STREQ( buffer2, "abab" );
}
TEST(InMemoryPolicyTests, SmallTests) {
    EXPECT_EQ( iris::fs::wfsPolicy.open("a.txt"), 0 );
    EXPECT_EQ( iris::fs::wfsPolicy.open("b.txt"), 1 );
    EXPECT_EQ( iris::fs::wfsPolicy.open("a.txt"), 0 );
    EXPECT_EQ( iris::fs::wfsPolicy.open("b.txt"), 1 );

    uint8_t data[4];
    for (int i = 0; i < 4; i ++) data[i] = i + 'a';

    int fd_a = 0;
    int fd_b = 1;
    iris::fs::wfsPolicy.write(fd_a, data, 2);
    iris::fs::wfsPolicy.write(fd_a, data, 2);
    iris::fs::wfsPolicy.write(fd_b, data, 4);

    EXPECT_EQ( iris::fs::rfsPolicy.open(0, "a.txt"), 0 );
    EXPECT_EQ( iris::fs::rfsPolicy.open(0, "a.txt"), 1 );
    EXPECT_EQ( iris::fs::rfsPolicy.open(0, "b.txt"), 2 );

    int rfa = 0; int rfb = 2;
    char buffer[5];
    buffer[4] = 0;

    EXPECT_EQ( iris::fs::rfsPolicy.read( rfa, (uint8_t*) buffer, 4 ), 4 );
    EXPECT_STREQ( buffer, "abab" );
    for (int i = 0; i < 5; i ++) buffer[i] = 0;
    EXPECT_EQ( iris::fs::rfsPolicy.read( rfb, (uint8_t*) buffer, 4 ), 4 );
    EXPECT_STREQ( buffer, "abcd" );
    for (int i = 0; i < 5; i ++) buffer[i] = 0;

    rfa = 1;
    char buffer2[7];
    for (int i = 0; i < 7; i ++) buffer2[i] = 0;

    EXPECT_EQ( iris::fs::rfsPolicy.read( rfa, (uint8_t*) buffer2, 2 ), 2 );
    EXPECT_EQ( iris::fs::rfsPolicy.read( rfa, (uint8_t*) (buffer2 + 2), 2 ), 2 );
    EXPECT_EQ( iris::fs::rfsPolicy.read( rfa, (uint8_t*) (buffer2 + 4), 2 ), 0 );
    EXPECT_STREQ( buffer2, "abab" );
}

TEST(InMemoryPolicyTests, BigTestsInternal) {
    int fd = iris::fs::memory::_internal::mem_storage.open_write("a.txt");

    uint8_t to_write[253];
    for (int i = 0; i < 253; i ++) to_write[i] = i;

    for (int i = 0; i < 20; i ++)
        iris::fs::memory::_internal::mem_storage.write(fd, to_write, 253);
    
    fd = iris::fs::memory::_internal::mem_storage.open_read("a.txt");
    int off = 0;
    uint8_t read[194];
    for (int i = 0; i < 20; i ++) {
        iris::fs::memory::_internal::mem_storage.read(fd, read, 194);

        for (int j = 0; j < 194; j ++) {
            EXPECT_EQ(read[j], off);
            off ++; off %= 253;
        }
    }
}
TEST(InMemoryPolicyTests, BigTests) {
    int fd = iris::fs::rfsPolicy.open(0, "a.txt");

    uint8_t to_write[253];
    for (int i = 0; i < 253; i ++) to_write[i] = i;

    for (int i = 0; i < 20; i ++)
        iris::fs::wfsPolicy.write(fd, to_write, 253);
    
    fd = iris::fs::rfsPolicy.open(0, "a.txt");
    int off = 0;
    uint8_t read[194];
    for (int i = 0; i < 20; i ++) {
        iris::fs::rfsPolicy.read(fd, read, 194);

        for (int j = 0; j < 194; j ++) {
            EXPECT_EQ(read[j], off);
            off ++; off %= 253;
        }
    }
}