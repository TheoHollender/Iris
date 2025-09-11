
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
#include <mutex>
#include <limits>
#include <cstdint>
#include <typeinfo>

#include "iris/core/registry.h"
#include "iris/fs/policy.h"

namespace iris::storage::type {

    using TypeIntType = uint32_t;

    constexpr TypeIntType POINTER_TYPE_ID = std::numeric_limits<TypeIntType>::max();
    #define TYPE_STORAGE_LOCATION "objects/types"

    const size_t prim_mark = std::numeric_limits<size_t>::max(); 

    enum Primitive : uint8_t {
        BOOL,
        
        CHAR,  UCHAR,
        SHORT, USHORT,
        INT,   UINT,
        LONG,  ULONG,
        LL,    ULL,

        FLOAT, DOUBLE
    };

    struct field {
        TypeIntType type;
        size_t offset;

        field (TypeIntType type, size_t offset) : type(type), offset(offset) {}
    };

    template<typename T>
    std::vector<field> fields_of_type ();

    struct TypeStorage {
        uint32_t fd = -1;
        bool storage_pendint_init = true;

        std::mutex write_mutex;
        std::mutex init_mutex;
        void init ();

        iris::core::Registry<const std::type_info*, TypeIntType> registry;

        void registerPrimitive (TypeIntType value, Primitive prim);
        
        template<typename T>
        void registerType (TypeIntType value) {
            init();
            std::vector<field> fields = fields_of_type<T>();

            std::lock_guard<std::mutex> lock(write_mutex);
            iris::fs::wfsPolicy.write(fd, (const uint8_t*) &value, sizeof(TypeIntType));

            size_t size_type = sizeof(T);
            iris::fs::wfsPolicy.write(fd, (const uint8_t*) &size_type, sizeof(size_t));

            size_t num_els = fields.size();
            iris::fs::wfsPolicy.write(fd, (const uint8_t*) &num_els, sizeof(size_t));
            
            for (auto &field : fields) {
                iris::fs::wfsPolicy.write(fd, (const uint8_t*) &field.type,   sizeof(TypeIntType));
                iris::fs::wfsPolicy.write(fd, (const uint8_t*) &field.offset, sizeof(size_t));
            }
        }
        
    public:
        template<typename T>
        TypeIntType get () {
            const auto [created, result] = registry.getAndIsNew(&typeid(T));
            if (!created) return result;

            registerType<T>(result);
            return result;
        }
    };
    
    #define MK_PRIMITIVE(TYPE, PRIM) \
        template<> \
        void TypeStorage::registerType<TYPE> (TypeIntType value); \
        template<> \
        void TypeStorage::registerType<const TYPE> (TypeIntType value);
    
    MK_PRIMITIVE(char, Primitive::CHAR);
    MK_PRIMITIVE(short, Primitive::SHORT);
    MK_PRIMITIVE(int, Primitive::INT);
    MK_PRIMITIVE(long, Primitive::LONG);
    MK_PRIMITIVE(long long, Primitive::LL);
    
    MK_PRIMITIVE(unsigned char, Primitive::UCHAR);
    MK_PRIMITIVE(unsigned short, Primitive::USHORT);
    MK_PRIMITIVE(unsigned int, Primitive::UINT);
    MK_PRIMITIVE(unsigned long, Primitive::ULONG);
    MK_PRIMITIVE(unsigned long long, Primitive::ULL);
    
    MK_PRIMITIVE(bool, Primitive::BOOL);
    MK_PRIMITIVE(float, Primitive::FLOAT);
    MK_PRIMITIVE(double, Primitive::DOUBLE);
    #undef MK_PRIMITIVE
    
    extern TypeStorage type_storage;

    #define IRIS_MAKE_TYPE_UUID(T) ([&]() {                              \
        static auto element(iris::storage::type::type_storage.get<T>()); \
        return element;                                                  \
    })()

    #define IRIS_FIELD(TYPE, SUBTYPE, MEMBER) field( IRIS_MAKE_TYPE_UUID(SUBTYPE), offsetof(TYPE, MEMBER) )
    #define IRIS_PTR_FIELD(TYPE, MEMBER) field( POINTER_TYPE_ID, offsetof(TYPE, MEMBER) )
}
