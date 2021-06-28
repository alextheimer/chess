#ifndef UTIL_BUFFER_H_
#define UTIL_BUFFER_H_

#include <cstdint>
#include "util/assert.h"


namespace util {

/*
Array that does *not* initialize its elements at its own initialization.
*/
template <typename T, std::size_t SIZE>
class Buffer {
 private:
    // Just stores raw bytes of each element.
    //    Note that initialization as a T array would construct every element.
    uint8_t arr_[SIZE * sizeof(T)];
 public:
    T& get(std::size_t index) const {
        ASSERT(index < SIZE, "index: " + std::to_string(index) + ", "
                            + "SIZE: " + std::to_string(SIZE));
        T* ptr = reinterpret_cast<T*>(const_cast<uint8_t*>(arr_));
        return ptr[index];
    }

    void set(std::size_t index, T& elt) {
        ASSERT(index < SIZE, "index: " + std::to_string(index) + ", "
                            + "SIZE: " + std::to_string(SIZE));
        T* ptr = reinterpret_cast<T*>(arr_);
        ptr[index] = elt;
    }

    /*
    Returns an Iterator at the first index.
    */
    T* start() const {
        return reinterpret_cast<T*>(const_cast<uint8_t*>(arr_));
    }
};

}

#endif  // UTIL_BUFFER_H_
