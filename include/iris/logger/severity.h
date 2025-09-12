
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
#include <cstdint>

enum class LogLevel : uint8_t {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

#if defined(IRIS_LOGLEVEL_DEBUG)
constexpr LogLevel IRIS_LOG_LEVEL = LogLevel::DEBUG;
#elif defined(IRIS_LOGLEVEL_INFO)
constexpr LogLevel IRIS_LOG_LEVEL = LogLevel::INFO;
#elif defined(IRIS_LOGLEVEL_WARN)
constexpr LogLevel IRIS_LOG_LEVEL = LogLevel::WARN;
#elif defined(IRIS_LOGLEVEL_ERROR)
constexpr LogLevel IRIS_LOG_LEVEL = LogLevel::ERROR;
#elif defined(IRIS_LOGLEVEL_FATAL)
constexpr LogLevel IRIS_LOG_LEVEL = LogLevel::FATAL;
#else
constexpr LogLevel IRIS_LOG_LEVEL = LogLevel::ERROR;
#endif

constexpr bool should_log (LogLevel level) {
    return level >= IRIS_LOG_LEVEL;
}
