#include <cassert>
#include <cstdint>
#include "board/board.h"
#include "util/bitops.h"

using namespace bitops;

bool validBitIndex(std::size_t bit_index) {
	return (bit_index < NUM_BITS) && (bit_index >= 0);
}

void bitops::setBit(BitOpType& bits, std::size_t bit_index, bool bit) {
    assert(validBitIndex(bit_index));
    std::size_t shift_count = (NUM_BITS - bit_index - 1);
    BitOpType mask = ~((BitOpType)1 << shift_count);
    bits &= mask;
    bits |= (static_cast<BitOpType>(bit) << shift_count);
}

bool bitops::getBit(BitOpType bits, size_t bit_index) {
    assert(validBitIndex(bit_index));
    std::size_t shift_count = (NUM_BITS - bit_index - 1);
    BitOpType mask = (BitOpType)1 << shift_count;
    BitOpType bitVal = (bits & mask) >> shift_count;
    assert((bitVal == 0) || (bitVal == 1));
    return static_cast<bool>(bitVal);
}

size_t bitops::popHighestBit(BitOpType& bits) {
	size_t bit_index = __builtin_clzl(bits);
	setBit(bits, bit_index, 0);
	return bit_index;
}
