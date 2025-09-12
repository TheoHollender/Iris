
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

#include "iris/logger/severity.h"
#include "iris/logger/format.h"
#include "iris/storage/args.h"

#include <mutex>

namespace iris::logger {

    namespace _internal {
        extern bool static_pending_init;
        extern std::mutex static_init_mutex;
        extern std::mutex static_write_mutex;
        extern uint32_t static_logger_names_fd;
        extern uint32_t static_logger_fd;
        extern uint32_t static_number_loggers;
    };

    #define LOGGER_NAMES_LOCATION "logger/names" 
    #define LOGGER_LOGS_LOCATION "logger/logs" 

    struct Logger {
    private:
        bool pending_init = true;
        const char* name  = nullptr;

        uint32_t logger_uuid;

        std::mutex init_mutex;
        void init ();
    public:
        constexpr Logger ();
        constexpr Logger (const char* name);
    
        void log (LogLevel level, FormatIntType fmt);
        void log (
            LogLevel level,
            FormatIntType fmt,
            iris::storage::args::Arguments args
        );
    };

    extern Logger rootLogger;

    #define IRIS_LOG(logger, level, fmt, ...) ([&]() { \
        if constexpr (should_log(level)) { \
            logger.log( \
                level, \
                IRIS_LOGGER_MAKE_STATIC_FORMAT(fmt) \
                __VA_OPT__(, IRIS_MAKE_ARGUMENTS(__VA_ARGS__)) \
            ); \
        } \
    }())

    #define IRIS_DEBUG(logger, fmt, ...) IRIS_LOG(logger, LogLevel::DEBUG, fmt __VA_OPT__(, __VA_ARGS__))
    #define IRIS_INFO(logger, fmt, ...)  IRIS_LOG(logger, LogLevel::INFO,  fmt __VA_OPT__(, __VA_ARGS__))
    #define IRIS_WARN(logger, fmt, ...)  IRIS_LOG(logger, LogLevel::WARN,  fmt __VA_OPT__(, __VA_ARGS__))
    #define IRIS_ERROR(logger, fmt, ...) IRIS_LOG(logger, LogLevel::ERROR, fmt __VA_OPT__(, __VA_ARGS__))
    #define IRIS_FATAL(logger, fmt, ...) IRIS_LOG(logger, LogLevel::FATAL, fmt __VA_OPT__(, __VA_ARGS__))

};
