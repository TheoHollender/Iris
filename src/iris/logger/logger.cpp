
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

#include "iris/logger/logger.h"

using namespace iris::logger;
using namespace iris::storage::args;

bool       _internal::static_pending_init    = true;
std::mutex _internal::static_init_mutex      = std::mutex();
std::mutex _internal::static_write_mutex     = std::mutex();
uint32_t   _internal::static_logger_fd       = -1;
uint32_t   _internal::static_logger_names_fd = -1;
uint32_t   _internal::static_number_loggers  = 0;

constexpr Logger::Logger () {}
constexpr Logger::Logger (const char* name) : name(name) {}

void static_init () {
    if (_internal::static_pending_init) {
        std::lock_guard<std::mutex> lock(_internal::static_init_mutex);
        if (!_internal::static_pending_init) return ;

        _internal::static_pending_init = false;
        _internal::static_logger_names_fd = iris::fs::wfsPolicy.open(LOGGER_NAMES_LOCATION);
        _internal::static_logger_fd = iris::fs::wfsPolicy.open(LOGGER_LOGS_LOCATION);
    }
}

#include <iostream>
void Logger::init () {
    static_init();

    if (pending_init) {
        std::lock_guard<std::mutex> lock(init_mutex);

        if (!pending_init) return ;
        pending_init = false;
        
        std::lock_guard<std::mutex> wlock(_internal::static_write_mutex);
        
        logger_uuid = _internal::static_number_loggers ++;
        iris::fs::wfsPolicy.write(_internal::static_logger_names_fd, (const uint8_t*) &logger_uuid, sizeof(uint32_t));
        
        uint32_t loggerNameSize = 0;
        if (name != nullptr)
            loggerNameSize = strlen(name);
        
        iris::fs::wfsPolicy.write(_internal::static_logger_names_fd, (const uint8_t*) &loggerNameSize, sizeof(uint32_t));
        if (name != nullptr)
            iris::fs::wfsPolicy.write(_internal::static_logger_names_fd, (const uint8_t*) name, loggerNameSize);
    }
}

void Logger::log (LogLevel level, FormatIntType fmt) {
    log(
        level,
        fmt,
        std::make_pair<
            iris::storage::args::ArgSchemeIntType,
            iris::storage::args::ArgValuesIntType
        >(
            std::numeric_limits<iris::storage::args::ArgSchemeIntType>::max(),
            std::numeric_limits<iris::storage::args::ArgValuesIntType>::max()
        )
    );
}
void Logger::log (
    LogLevel level,
    FormatIntType fmt,
    iris::storage::args::Arguments args
) {
    init();

    constexpr size_t bufferSize = 
        sizeof(uint32_t)
      + sizeof(LogLevel)
      + sizeof(FormatIntType)
      + sizeof(ArgSchemeIntType)
      + sizeof(ArgValuesIntType);
    uint8_t buffer[bufferSize];

    uint8_t* current = buffer;
    *((uint32_t*) (current)) = logger_uuid; current += sizeof(uint32_t);
    *((LogLevel*) (current)) = level; current += sizeof(LogLevel);
    *((FormatIntType*) (current)) = fmt; current += sizeof(FormatIntType);
    *((ArgSchemeIntType*) (current)) = args.first; current += sizeof(ArgSchemeIntType);
    *((ArgValuesIntType*) (current)) = args.second;

    std::lock_guard<std::mutex> lock(_internal::static_write_mutex);
    iris::fs::wfsPolicy.write(_internal::static_logger_fd, buffer, bufferSize);
}

Logger iris::logger::rootLogger = Logger();
