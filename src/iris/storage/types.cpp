
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

#include "iris/storage/types.h"

using namespace iris::storage::type;

#define MK_PRIMITIVE(TYPE, PRIM) \
    template<> \
    void TypeStorage::registerType<TYPE> (TypeIntType value) { registerPrimitive (value, PRIM); } \
    template<> \
    void TypeStorage::registerType<const TYPE> (TypeIntType value) { registerPrimitive (value, PRIM); }

void TypeStorage::init() {
    if (storage_pendint_init) {
        std::lock_guard<std::mutex> lock(init_mutex);
        if (!storage_pendint_init) return ;

        fd = iris::fs::wfsPolicy.open(TYPE_STORAGE_LOCATION);
    }
}
void TypeStorage::registerPrimitive (TypeIntType value, Primitive prim) {
    init();

    std::lock_guard<std::mutex> lock(write_mutex);

    iris::fs::wfsPolicy.write(fd, (const uint8_t*) &value,     sizeof(TypeIntType));
    iris::fs::wfsPolicy.write(fd, (const uint8_t*) &prim_mark, sizeof(size_t));
    iris::fs::wfsPolicy.write(fd, (const uint8_t*) &prim,      sizeof(Primitive));
}

TypeStorage iris::storage::type::type_storage = TypeStorage();

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
