#include <cstdint>
#include "board/board.h"
#include "util/bitops.h"

#include "util/assert.h"

// TODO(theimer): bit index 0 should be least significant

bool isValidBitIndex(std::size_t bit_index) {
    return (bit_index < util::NUM_BITOP_BITS) && (bit_index >= 0);
}

void util::setBit(BitOpType* bits, std::size_t bit_index, bool bit) {
    ASSERT(isValidBitIndex(bit_index), "index: " + std::to_string(bit_index));
    // make a mask of all 1's except at the index
    std::size_t shift_count = (util::NUM_BITOP_BITS - bit_index - 1);
    BitOpType mask = ~((BitOpType)1 << shift_count);
    // zero out the bit at the index
    *bits &= mask;
    // "or" in the new bit
    *bits |= (static_cast<BitOpType>(bit) << shift_count);
}

bool util::getBit(BitOpType bits, std::size_t bit_index) {
    ASSERT(isValidBitIndex(bit_index), "index: " + std::to_string(bit_index));
    // make a mast of all 0's except at the index
    std::size_t shift_count = (util::NUM_BITOP_BITS - bit_index - 1);
    BitOpType mask = (BitOpType)1 << shift_count;
    // "and" it with the bits, then shift it to the least-significant index
    BitOpType bitVal = (bits & mask) >> shift_count;
    ASSERT((bitVal == 0) || (bitVal == 1), "bitVal: " + std::to_string(bitVal));
    return static_cast<bool>(bitVal);
}

size_t util::popHighestBit(BitOpType* bits) {
    ASSERT(*bits > 0, "cannot pop bits from 0");
    std::size_t bit_index = __builtin_clzl(static_cast<unsigned long>(*bits));
    setBit(bits, bit_index, 0);
    return bit_index;
}
