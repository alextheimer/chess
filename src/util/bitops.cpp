#include <cassert>
#include "globs.h"
#include "util/bitops.h"

void bitops::set_bit(uint64_t& bits, size_t bit_index, size_t bit) {
    assert((bit_index < globs::BOARD_SIZE) && (bit_index >= 0));
    unsigned int shift_count = (globs::BOARD_SIZE - bit_index - 1);
    unsigned int mask = ~((uint64_t)1 << shift_count);
    bits &= mask;
    bit <<= shift_count;
    bits |= bit;
}

size_t bitops::get_bit(uint64_t bits, size_t bit_index) {
    assert((bit_index < globs::BOARD_SIZE) && (bit_index >= 0));
    unsigned int shift_count = (globs::BOARD_SIZE - bit_index - 1);
    unsigned int mask = (uint64_t)1 << shift_count;
    return (bits & mask) >> shift_count;
}
