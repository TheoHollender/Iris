
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

#include "iris/storage/object.h"

using namespace iris::storage::object;

void ObjectGlobalStorage::init () {
    if (storage_pendint_init) {
        std::lock_guard<std::mutex> lock(init_mutex);
        if (!storage_pendint_init) return ;

        fd = iris::fs::wfsPolicy.open(OBJECT_STORAGE_LOCATION);
    }
}

#define MK_PRIMITIVE(TYPE) \
    template<> \
    void iris::storage::object::prepare_object (const TYPE& t) {}

MK_PRIMITIVE(char)
MK_PRIMITIVE(short)
MK_PRIMITIVE(int)
MK_PRIMITIVE(long)
MK_PRIMITIVE(long long)

MK_PRIMITIVE(unsigned char)
MK_PRIMITIVE(unsigned short)
MK_PRIMITIVE(unsigned int)
MK_PRIMITIVE(unsigned long)
MK_PRIMITIVE(unsigned long long)

MK_PRIMITIVE(float)
MK_PRIMITIVE(double)
MK_PRIMITIVE(bool)

ObjectGlobalStorage iris::storage::object::storageContainer = ObjectGlobalStorage();
