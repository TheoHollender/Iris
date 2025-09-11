
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
#include "iris/storage/argscheme.h"
#include "iris/storage/object.h"

namespace iris::storage::args {
    using ArgValuesIntType = uint32_t;
    using ArgValuesStorage = iris::storage::array::ArrayStorage<
        iris::storage::object::ObjectIntType,
        ArgValuesIntType
    >;

    extern ArgValuesStorage arg_values_storage;
    #define ARG_VALUES_LOCATION "args/objlist"

    template<typename... Args>
    constexpr ArgSchemeIntType make_argvalues (Args... args) {
        return iris::storage::args::arg_values_storage.store( IRIS_STORE_OBJECT(args)... );
    }

    #define IRIS_MAKE_ARGVALUES(...) (iris::storage::args::make_argvalues(__VA_ARGS__))
    
    template<typename T>
    struct argument {
        const char* label;
        const T& value;

        argument (const char* label, const T& value) : label(label), value(value) {}
    };
    #define IRIS_ARGUMENT(label, value) iris::storage::args::argument(label, value)

    template<typename FuncLabel, typename FuncValue, typename... Args>
    constexpr std::pair<ArgSchemeIntType, ArgValuesIntType> generate_args (
        FuncLabel funcLabels, FuncValue funcValues, Args ...args) {
        return std::make_pair<ArgSchemeIntType, ArgValuesIntType>(
            funcLabels(args.label...),
            funcValues(args.value...)
        );
    }

    /**
     * Internal way to create arguments using optimizations defined by X and Y
     */
    #define IRIS_MAKE_ARGUMENTS_SXDY(X, Y, ...) generate_args([](auto... labels) {\
        X element( IRIS_MAKE_ARGSCHEME(labels...) );\
        return element;\
    }, [](auto... values) {\
        Y element( IRIS_MAKE_ARGVALUES(values...) ); \
        return element; \
    }, __VA_ARGS__)
    
    /**
     * Create the argument lists for the given parameters.
     *  - Only works if labels and values are constant
     * 
     * If labels or values aren't constant, please refer to one of
     * IRIS_MAKE_ARGUMENTS_SNSO, IRIS_MAKE_ARGUMENTS_DNSO,
     * IRIS_MAKE_ARGUMENTS_SNDO or IRIS_MAKE_ARGUMENTS_DNDO
     */
    #define IRIS_MAKE_ARGUMENTS_SNSO(...) IRIS_MAKE_ARGUMENTS_SXDY(static auto, static auto, __VA_ARGS__)
    /**
     * Create the argument lists for the given parameters.
     *  - Only works if labels are constant
     * 
     * If labels aren't constant, please refer to one of
     * IRIS_MAKE_ARGUMENTS_SNSO, IRIS_MAKE_ARGUMENTS_DNSO,
     * IRIS_MAKE_ARGUMENTS_SNDO or IRIS_MAKE_ARGUMENTS_DNDO
     */
    #define IRIS_MAKE_ARGUMENTS_SNDO(...) IRIS_MAKE_ARGUMENTS_SXDY(static auto, auto, __VA_ARGS__)
    /**
     * Create the argument lists for the given parameters.
     *  - Only works if values are constant
     * 
     * If values aren't constant, please refer to one of
     * IRIS_MAKE_ARGUMENTS_SNSO, IRIS_MAKE_ARGUMENTS_DNSO,
     * IRIS_MAKE_ARGUMENTS_SNDO or IRIS_MAKE_ARGUMENTS_DNDO
     */
    #define IRIS_MAKE_ARGUMENTS_DNSO(...) IRIS_MAKE_ARGUMENTS_SXDY(auto, static auto, __VA_ARGS__)
    /**
     * Create the argument lists for the given parameters.
     * 
     * If label or values are constant, please refer to one of.
     * IRIS_MAKE_ARGUMENTS_SNSO, IRIS_MAKE_ARGUMENTS_DNSO,
     * IRIS_MAKE_ARGUMENTS_SNDO or IRIS_MAKE_ARGUMENTS_DNDO
     */
    #define IRIS_MAKE_ARGUMENTS_DNDO(...) IRIS_MAKE_ARGUMENTS_SXDY(auto, auto, __VA_ARGS__)

    /**
     * Create the argument lists for the given parameters.
     *  - Only works if labels are constant
     * 
     * If labels aren't constant, please refer to one of
     * IRIS_MAKE_ARGUMENTS_SNSO, IRIS_MAKE_ARGUMENTS_DNSO,
     * IRIS_MAKE_ARGUMENTS_SNDO or IRIS_MAKE_ARGUMENTS_DNDO
     */
    #define IRIS_MAKE_ARGUMENTS(...) IRIS_MAKE_ARGUMENTS_SNDO(__VA_ARGS__)
}