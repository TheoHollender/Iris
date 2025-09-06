
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

#include "iris/storage/strings.h"

namespace iris::logger {

    /**
     * Definition of the string registry as well as details on where to store formats.
     */

    using FormatRegistry = iris::storage::StringStorageRegistry<uint64_t>;
    extern FormatRegistry format_registry;

    #define FORMAT_STORAGE_LOCATION "logs/formats"

    /**
     * Get an element from the format registry in a static way.
     *   The target should be a constant expression.
     *   The call to the underlying map is done once statically.
     * 
     * This is good for highly intensive programs as the CPU will be able
     *   to predict correctly where the information is stored and this should
     *   be close to O(1).
     */
    #define IRIS_LOGGER_MAKE_STATIC_FORMAT(format) ([]() {    \
        static uint64_t element(iris::logger::format_registry.get(format));                \
        return element.content;                                                 \
    })()

    /**
     * Get an element from the format registry. If the element is known in advance,
     *   you should use IRIS_LOGGER_MAKE_STATIC_FORMAT, which will be much faster.
     */
    #define IRIS_LOGGER_MAKE_FORMAT(format) (iris::logger::format_registry.get(format))


}