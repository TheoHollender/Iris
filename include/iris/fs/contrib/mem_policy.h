
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

#include <stdint.h>
#include <vector>
#include <string>

#include "iris/core/registry.h"

namespace iris::fs::memory {

    #ifndef IRIS_FS_MEM_PAGE_BITSIZE
    #define IRIS_FS_MEM_PAGE_BITSIZE 12
    #endif
    
    const uint32_t IRIS_FS_MEM_PAGE_SIZE   = 1 << IRIS_FS_MEM_PAGE_BITSIZE;
    const uint32_t IRIS_FS_MEM_OFFSET_MASK = IRIS_FS_MEM_PAGE_SIZE - 1;

    namespace _internal {

        #ifndef IRIS_FS_MEM_TESTING
        extern bool displayed_warning;
        void warning_message ();
        #endif

        struct InMemoryPage {
            uint8_t  *ptr;
            uint32_t size;

            InMemoryPage ();
        };

        struct InMemoryLocation {
            uint32_t fd;
            uint32_t offset;
        };

        struct InMemoryBuffer {
        private:
            std::vector<InMemoryPage> pages;

            uint32_t total_size     = 0;
            uint32_t total_capacity = 0;
        public:
            void     write (uint8_t* ptr, uint32_t size);
            uint32_t read  (uint8_t* ptr, uint32_t size, InMemoryLocation &location);
        };

        void create_in_memory_buffer (const std::string &path, uint32_t fd);

        struct InMemoryStorage {
        private:
            iris::core::Registry<std::string, uint32_t> storage_registry
                = iris::core::Registry<std::string, uint32_t>(&create_in_memory_buffer);

            std::vector<InMemoryBuffer>   buffers;
            std::vector<InMemoryLocation> read_locations;
        public:
            InMemoryStorage ();

            void add_buffer (uint32_t fd);

            uint32_t open_write (const std::string &path);
            uint32_t open_read  (const std::string &path);

            void     write (uint32_t fd, uint8_t* ptr, uint32_t size);
            uint32_t read  (uint32_t fd, uint8_t* ptr, uint32_t size);
        };

        extern InMemoryStorage mem_storage;
    }

}