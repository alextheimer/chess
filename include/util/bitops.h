#ifndef BITOPS_H_
#define BITOPS_H_

#include <cstdint>

using std::size_t;

namespace bitops {
    void set_bit(uint64_t& bits, size_t bit_index, size_t bit);
    size_t get_bit(uint64_t bits, size_t bit_index);
}

#endif  // BITOPS_H_