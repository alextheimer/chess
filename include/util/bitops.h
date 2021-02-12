#ifndef BITOPS_H_
#define BITOPS_H_

#include <cstdint>

namespace bitops {
    void set_bit(uint64_t& bits, std::size_t bit_index, std::size_t bit);
    std::size_t get_bit(uint64_t bits, std::size_t bit_index);
}

#endif  // BITOPS_H_
