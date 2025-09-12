
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
#include "iris/fs/policy.h"
#include "iris/storage/types.h"

#include <cstdint>
#include <vector>
#include <mutex>

using namespace iris::storage::type;

namespace iris::storage::object {

    /**
     * This method is meant to be used to register
     *   the objects subarrays into the Object
     *   Pointer Storage. No STL objects are
     *   supported in the OPS.
     * 
     * It is not recommended to have any indirections in the
     *   Object Global Storage, but the feature exists.
     * 
     * It is really important for objects to be Non-Recursive.
     */
    template<typename T>
    void prepare_object (const T &t);

    using ObjectIntType = uint32_t;
    #define OBJECT_STORAGE_LOCATION "objects/instances"

    struct ObjectGlobalStorage {
    private:
        uint32_t fd = -1;
        ObjectIntType uuid = 0;
        bool storage_pendint_init = true;

        std::mutex write_mutex;
        std::mutex init_mutex;
        void init ();

    public:
        template<typename T>
        ObjectIntType registerObject (const T &object) {
            init();
            prepare_object<T>(object);
            
            std::lock_guard<std::mutex> lock(write_mutex);

            TypeIntType type = IRIS_MAKE_TYPE_UUID(T);
            const size_t objsize = sizeof(T);
            iris::fs::wfsPolicy.write(fd, (const uint8_t*) &type, sizeof(TypeIntType));
            iris::fs::wfsPolicy.write(fd, (const uint8_t*) &uuid, sizeof(ObjectIntType));
            iris::fs::wfsPolicy.write(fd, (const uint8_t*) &objsize, sizeof(size_t));
            iris::fs::wfsPolicy.write(fd, (const uint8_t*) &object, objsize);

            return uuid ++;
        }
        template<typename T>
        void registerPointer (const T* pointer, size_t num_objects, bool contains_pointer) {
            init();
            std::vector<ObjectIntType> content(num_objects);
            
            for (size_t offset = 0; offset < num_objects; offset ++)
                content[offset] = registerObject<T>(pointer[offset]);
            
            std::lock_guard<std::mutex> lock(write_mutex);

            TypeIntType pointerType = POINTER_TYPE_ID;
            size_t ptr_loc = (size_t) pointer;
            iris::fs::wfsPolicy.write(fd, (const uint8_t*) &pointerType, sizeof(TypeIntType));
            iris::fs::wfsPolicy.write(fd, (const uint8_t*) &contains_pointer, sizeof(bool));
            iris::fs::wfsPolicy.write(fd, (const uint8_t*) &num_objects, sizeof(size_t));
            iris::fs::wfsPolicy.write(fd, (const uint8_t*) &ptr_loc, sizeof(size_t));
            iris::fs::wfsPolicy.write(
                fd,
                (const uint8_t*) content.data(),
                num_objects * sizeof(ObjectIntType));
        }
    };

    extern ObjectGlobalStorage storageContainer;

    #define IRIS_STORE_OBJECT(o) (iris::storage::object::storageContainer.registerObject(o))
    #define IRIS_STORE_POINTER(ptr, sze, cpt) (iris::storage::object::storageContainer.registerPointer(ptr, sze, cpt))

}
