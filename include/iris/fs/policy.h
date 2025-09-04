
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
#include <string>

/**
 * The Iris FileSystem consists of two main systems
 *  - The Write Policy, a High-Performance write-only
 *    Unix-like interface, supposed to be run during the
 *    telemetry collection
 *  - The Read Policy, a read-only Unix-like interface
 *    supposed to run during the telemetry extraction.
 */

namespace iris::fs {

    struct WriteFileSystemPolicy {
    public:
        uint32_t open  (std::string path);
        void     write (uint32_t fd, uint8_t* ptr, uint32_t size);
    
        void flush (uint32_t fd);
        void flush_all ();
    };

    extern WriteFileSystemPolicy wfsPolicy;

    struct ReadFileSystemPolicy {
    public:
        uint32_t open (std::string path);
        uint32_t read (uint32_t fd, uint8_t* ptr, uint32_t size);
    };

    extern ReadFileSystemPolicy rfsPolicy;

}
