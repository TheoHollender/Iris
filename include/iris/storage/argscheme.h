
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
#include "iris/storage/strings.h"
#include "iris/storage/array.h"
#include <type_traits>

namespace iris::storage::args {

    /**
     * Definition of the string registry as well as details on where to store argument names.
     */
    using ArgNameIntType  = uint32_t;
    using ArgNameRegistry = iris::storage::StringStorageRegistry<ArgNameIntType>;
    extern ArgNameRegistry arg_name_registry;

    #define ARG_NAMES_LOCATION "args/names"

    #define IRIS_MAKE_ARGNAME(name) (iris::storage::args::arg_name_registry.get(name))
    #define IRIS_MAKE_STATIC_ARGNAME(name) ([]() {     \
        static auto element(IRIS_MAKE_ARGNAME(name));  \
        return element.content;                        \
    })()

    /**
     * Definition of the scheme storage as well as details on where to store argument schemes.
     */
    using ArgSchemeIntType = uint32_t;
    using ArgSchemeStorage = iris::storage::array::ArrayStorage<ArgNameIntType, ArgSchemeIntType>;
    extern ArgSchemeStorage arg_scheme_storage;

    #define ARG_SCHEME_LOCATION "args/schemes"

    template<typename... Args>
    constexpr ArgSchemeIntType make_argscheme (Args... args)
        requires (std::is_convertible_v<const char*, Args> && ...) {
        return iris::storage::args::arg_scheme_storage.store( IRIS_MAKE_ARGNAME(args)... );
    }

    #define IRIS_MAKE_ARGSCHEME(...) (iris::storage::args::make_argscheme(__VA_ARGS__))
    #define IRIS_MAKE_STATIC_ARGSCHEME(...) {                  \
        static auto element(IRIS_MAKE_ARGSCHEME(__VA_ARGS__)); \
        return element;                                        \
    }
}
