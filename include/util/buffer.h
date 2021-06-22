#ifndef UTIL_BUFFER_H_
#define UTIL_BUFFER_H_

#include <cstdint>
#include "util/assert.h"


namespace util {

// TODO(theimer): overuse of this will bloat the binary, no?
//    Do any better solutions exist in C++??

/**
 * Array that does not initialize its elements at its own initialization.
 */
template <typename T, std::size_t size>
class Buffer {
 private:
    uint8_t arr_[size * sizeof(T)];
 public:
    T& get(std::size_t index) {
        ASSERT(index < size, "index: " + std::to_string(index) + ", "
                            + "size: " + std::to_string(size));
        T * ptr = reinterpret_cast<T*>(arr_);
        return ptr[index];
    }

    void set(std::size_t index, T& elt) {
        ASSERT(index < size, "index: " + std::to_string(index) + ", "
                            + "size: " + std::to_string(size));
        T * ptr = reinterpret_cast<T*>(arr_);
        ptr[index] = elt;
    }

    T * start() {
        return reinterpret_cast<T*>(arr_);
    }
};

}

#endif  // UTIL_BUFFER_H_
