#include "util/buffer.h"

#include <cassert>

using namespace util;

template <typename T, std::size_t count>
T& Buffer<T, count>::get(std::size_t index) {
    assert(index < count);
    T * ptr = reinterpret_cast<T*>(arr_);
    return ptr[index];
}

template <typename T, std::size_t count>
void Buffer<T, count>::set(std::size_t index, T& elt) {
    assert(index < count);
    T * ptr = reinterpret_cast<T*>(arr_);
    ptr[index] = elt;
}
