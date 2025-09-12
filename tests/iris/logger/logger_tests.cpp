
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
#include "iris/logger/logger.h"

#include "utils/fsverify.h"

using namespace iris::logger;
using namespace iris::storage::args;
using namespace iris::storage::object;
using namespace iris::storage::type;

TEST(LoggerTests, TestNoArgumentsLogger) {
    IRIS_FATAL(rootLogger, "A fatal error occured.");

    int fdFormat = iris::fs::rfsPolicy.open(FORMAT_STORAGE_LOCATION);
    VERIFY_FD(
        fdFormat,
        static_cast<FormatIntType>(0),
        static_cast<size_t>(22),
        'A', ' ', 'f', 'a', 't', 'a', 'l', ' ', 'e', 'r', 'r', 'o', 'r',
        ' ', 'o', 'c', 'c', 'u', 'r', 'e', 'd', '.'
    );
    VERIFY_EOF_FD(fdFormat);

    int fdLoggerNames = iris::fs::rfsPolicy.open(LOGGER_NAMES_LOCATION);
    VERIFY_FD(
        fdLoggerNames,
        static_cast<uint32_t>(0),
        static_cast<uint32_t>(0)
    );
    VERIFY_EOF_FD(fdLoggerNames);
    
    int fdLogger = iris::fs::rfsPolicy.open(LOGGER_LOGS_LOCATION);
    VERIFY_FD(
        fdLogger,
        static_cast<uint32_t>(0),
        static_cast<LogLevel>(LogLevel::FATAL),
        static_cast<FormatIntType>(0),
        static_cast<ArgSchemeIntType>(std::numeric_limits<ArgSchemeIntType>::max()),
        static_cast<ArgValuesIntType>(std::numeric_limits<ArgValuesIntType>::max())
    );
    VERIFY_EOF_FD(fdLogger);

    int fdArgValues  = iris::fs::rfsPolicy.open(ARG_VALUES_LOCATION);
    int fdArgSchemes = iris::fs::rfsPolicy.open(ARG_SCHEME_LOCATION);
    int fdArgNames   = iris::fs::rfsPolicy.open(ARG_NAMES_LOCATION);
    VERIFY_EOF_FD(fdArgValues);
    VERIFY_EOF_FD(fdArgNames);
    VERIFY_EOF_FD(fdArgSchemes);
    
    int fdTypes = iris::fs::rfsPolicy.open(TYPE_STORAGE_LOCATION);
    int fdObjects = iris::fs::rfsPolicy.open(OBJECT_STORAGE_LOCATION);
    VERIFY_EOF_FD(fdTypes);
    VERIFY_EOF_FD(fdObjects);
}
TEST(LoggerTests, TestDisabledLogger) {
    IRIS_DEBUG(rootLogger, "Some debug information.");

    int fdFormat = iris::fs::rfsPolicy.open(FORMAT_STORAGE_LOCATION);
    VERIFY_EOF_FD(fdFormat);

    int fdLoggerNames = iris::fs::rfsPolicy.open(LOGGER_NAMES_LOCATION);
    VERIFY_EOF_FD(fdLoggerNames);
    
    int fdLogger = iris::fs::rfsPolicy.open(LOGGER_LOGS_LOCATION);
    VERIFY_EOF_FD(fdLogger);

    int fdArgValues  = iris::fs::rfsPolicy.open(ARG_VALUES_LOCATION);
    int fdArgSchemes = iris::fs::rfsPolicy.open(ARG_SCHEME_LOCATION);
    int fdArgNames   = iris::fs::rfsPolicy.open(ARG_NAMES_LOCATION);
    VERIFY_EOF_FD(fdArgValues);
    VERIFY_EOF_FD(fdArgNames);
    VERIFY_EOF_FD(fdArgSchemes);
    
    int fdTypes = iris::fs::rfsPolicy.open(TYPE_STORAGE_LOCATION);
    int fdObjects = iris::fs::rfsPolicy.open(OBJECT_STORAGE_LOCATION);
    VERIFY_EOF_FD(fdTypes);
    VERIFY_EOF_FD(fdObjects);
}

TEST(LoggerTests, TestArgumentsLogger) {
    for (size_t off = 0; off < 8; off ++) {
        IRIS_FATAL(
            rootLogger,
            "A fatal error occured.",
            IRIS_ARGUMENT("lab1", static_cast<size_t>(off))
        );
    }

    int fdFormat = iris::fs::rfsPolicy.open(FORMAT_STORAGE_LOCATION);
    VERIFY_FD(
        fdFormat,
        static_cast<FormatIntType>(0),
        static_cast<size_t>(22),
        'A', ' ', 'f', 'a', 't', 'a', 'l', ' ', 'e', 'r', 'r', 'o', 'r',
        ' ', 'o', 'c', 'c', 'u', 'r', 'e', 'd', '.'
    );
    VERIFY_EOF_FD(fdFormat);

    int fdLoggerNames = iris::fs::rfsPolicy.open(LOGGER_NAMES_LOCATION);
    VERIFY_FD(
        fdLoggerNames,
        static_cast<uint32_t>(0),
        static_cast<uint32_t>(0)
    );
    VERIFY_EOF_FD(fdLoggerNames);
    
    int fdLogger = iris::fs::rfsPolicy.open(LOGGER_LOGS_LOCATION);
    for (size_t off = 0; off < 8; off ++) {
        VERIFY_FD(
            fdLogger,
            static_cast<uint32_t>(0),
            static_cast<LogLevel>(LogLevel::FATAL),
            static_cast<FormatIntType>(0),
            static_cast<ArgSchemeIntType>(0),
            static_cast<ArgValuesIntType>(off)
        );
    }
    VERIFY_EOF_FD(fdLogger);

    int fdArgValues  = iris::fs::rfsPolicy.open(ARG_VALUES_LOCATION);
    int fdArgSchemes = iris::fs::rfsPolicy.open(ARG_SCHEME_LOCATION);
    int fdArgNames   = iris::fs::rfsPolicy.open(ARG_NAMES_LOCATION);
    for (size_t off = 0; off < 8; off ++) {
        VERIFY_FD(
            fdArgValues,
            static_cast<size_t>(1),
            static_cast<ObjectIntType>(off) );
    }
    VERIFY_EOF_FD(fdArgValues);
    VERIFY_FD( fdArgNames, static_cast<ArgNameIntType>(0), 4ULL, 'l', 'a', 'b', '1' );
    VERIFY_EOF_FD(fdArgNames);
    VERIFY_FD( fdArgSchemes, static_cast<size_t>(1), static_cast<ArgNameIntType>(0) );
    VERIFY_EOF_FD(fdArgSchemes);
    
    int fdTypes = iris::fs::rfsPolicy.open(TYPE_STORAGE_LOCATION);
    int fdObjects = iris::fs::rfsPolicy.open(OBJECT_STORAGE_LOCATION);
    VERIFY_FD( fdTypes, static_cast<TypeIntType>(0), prim_mark, Primitive::ULONG);
    VERIFY_EOF_FD(fdTypes);
    for (size_t off = 0; off < 8; off ++) {
        VERIFY_FD(
            fdObjects,
            static_cast<TypeIntType>(0), static_cast<ObjectIntType>(off),
            static_cast<size_t>(sizeof(size_t)), static_cast<size_t>(off) );
    }
    VERIFY_EOF_FD(fdObjects);
}
