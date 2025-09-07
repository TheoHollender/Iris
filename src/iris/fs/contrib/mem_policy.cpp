
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

#include <iostream>
#include <string.h>

#include "iris/fs/policy.h"
#include "iris/fs/contrib/mem_policy.h"

using namespace iris::fs;
using namespace iris::fs::memory;

#ifndef IRIS_FS_MEM_TESTING
bool _internal::displayed_warning = false;
void _internal::warning_message () {
    if (displayed_warning) return ;
    displayed_warning = true;

    std::cout << "[iris/fs/mem] You are currently using an InMemory fs policy.\n";
    std::cout << "  This means all the data collected is stored in memory.\n";
    std::cout << "  This implementation is only intended for testing purposes.\n";
    std::cout << "  IT DOES NOT PROVIDE ANY WAY TO RECOVER FROM AN OUT OF MEMORY\n";
    std::cout << "  ERROR. IT DOES NOT FREE ANY OF THE ALLOCATED MEMORY DURING\n";
    std::cout << "  EXECUTION, LEADING TO OUT OF MEMORY OVER LONG PERIODS OF TIME.\n";
    std::cout << "  \n";
    std::cout << "  PLEASE DO NOT USE THAT IMPLEMENTATION UNLESS IT IS FOR TESTING.\n";
    std::cout << "  \n";
    std::cout << "  You may disable this warning by passing the flag IRIS_FS_MEM_TESTING, or\n";
    std::cout << "  Linking your program with 'iris_testing' instead of 'iris'.\n";
    std::cout << "  \n";
}

#define IRIS_FS_MEM_WARNING() _internal::warning_message()
#else
#define IRIS_FS_MEM_WARNING()
#endif

/**
 * Implementation of internal objects.
 */
uint32_t _internal::InMemoryBuffer::read (uint8_t *ptr, uint32_t size, _internal::InMemoryLocation &location) {
    uint32_t total_read = 0;

    while (1) {
        if (location.offset >= total_size || size == 0) break ;
    
        uint32_t page_id  = location.offset >> IRIS_FS_MEM_PAGE_BITSIZE;
        uint32_t page_off = location.offset &  IRIS_FS_MEM_OFFSET_MASK;

        uint32_t page_rem = std::min(
            std::min(size, pages[page_id].size - page_off),
            total_size - location.offset
        );
        memcpy(ptr, pages[page_id].ptr + page_off, page_rem);
        
        ptr += page_rem;
        size -= page_rem;
        location.offset += page_rem;
        total_read += page_rem;
    }

    return total_read;
}

void _internal::InMemoryBuffer::write (const uint8_t *ptr, uint32_t size) {
    while (size > 0) {
        if (total_capacity == total_size) {
            total_capacity += IRIS_FS_MEM_PAGE_SIZE;

            pages.push_back( InMemoryPage() );
        }

        uint32_t to_write = std::min(size, total_capacity - total_size);
        uint32_t page_off = total_size & IRIS_FS_MEM_OFFSET_MASK;

        memcpy(pages.back().ptr + page_off, ptr, to_write);

        total_size += to_write;

        ptr  += to_write;
        size -= to_write;
    }
}

_internal::InMemoryPage::InMemoryPage () {
    size = IRIS_FS_MEM_PAGE_SIZE;
    ptr  = (uint8_t*) malloc( IRIS_FS_MEM_PAGE_SIZE );

    if (ptr == NULL) {
        // LCOV_EXCL_START
        std::cout << "[iris/fs/memory] OUT_OF_MEMORY error of the iris::fs::memory policy. \n";
        std::cout << "  This error message occurs as you system does not have enough memory to store\n";
        std::cout << "  all your logs / traces / metrics in memory. Since this is a virtual file system\n";
        std::cout << "  policy designed for testing and not for production applications, all data was lost.\n";
        std::cout << "  \n";

        #ifndef IRIS_FS_MEM_TESTING
        std::cout << "  Please refer to the below warning to understand the details of the in memory policy.\n";
        std::cout << "  \n";

        displayed_warning = false;
        IRIS_FS_MEM_WARNING();
        #endif

        exit(1);
        // LCOV_EXCL_STOP
    }
}

uint32_t _internal::InMemoryStorage::open_write (const std::string &path) {
    return storage_registry.get(path);
}
uint32_t _internal::InMemoryStorage::open_read  (const std::string &path) {
    uint32_t fd = storage_registry.get(path);

    read_locations.push_back({
        .fd = fd,
        .offset = 0
    });
    return read_locations.size() - 1;
}

void _internal::InMemoryStorage::write (uint32_t fd, const uint8_t* ptr, uint32_t size) {
    return buffers[fd].write(ptr, size);
}
uint32_t _internal::InMemoryStorage::read  (uint32_t fd, uint8_t* ptr, uint32_t size) {
    _internal::InMemoryLocation &location = read_locations[fd];
    
    return buffers[location.fd].read(ptr, size, location);
}

void _internal::InMemoryStorage::add_buffer (uint32_t fd) {
    while (fd >= buffers.size())
        buffers.push_back(InMemoryBuffer());
}
void _internal::create_in_memory_buffer (const std::string &path, uint32_t fd) {
    _internal::mem_storage.add_buffer(fd);
}
// LCOV_EXCL_START
_internal::InMemoryStorage::InMemoryStorage () {}
// LCOV_EXCL_STOP

_internal::InMemoryStorage _internal::mem_storage;

/**
 * Base implementation of wfs and rfs policies.
 */

uint32_t WriteFileSystemPolicy::open (std::string path) {
    IRIS_FS_MEM_WARNING();
    return _internal::mem_storage.open_write(path);
}
void WriteFileSystemPolicy::write (uint32_t fd, const uint8_t* ptr, uint32_t size) {
    IRIS_FS_MEM_WARNING();

    return _internal::mem_storage.write(fd, ptr, size);
}
// LCOV_EXCL_START
void WriteFileSystemPolicy::flush (uint32_t fd) {
    IRIS_FS_MEM_WARNING();
}
void WriteFileSystemPolicy::flush_all () {
    IRIS_FS_MEM_WARNING();
}
// LCOV_EXCL_STOP

uint32_t ReadFileSystemPolicy::open (std::string path) {
    IRIS_FS_MEM_WARNING();

    return _internal::mem_storage.open_read(path);
}
uint32_t ReadFileSystemPolicy::read (uint32_t fd, uint8_t* ptr, uint32_t size) {
    IRIS_FS_MEM_WARNING();
    
    return _internal::mem_storage.read(fd, ptr, size);
}

WriteFileSystemPolicy iris::fs::wfsPolicy;
ReadFileSystemPolicy  iris::fs::rfsPolicy;
