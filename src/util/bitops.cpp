#include <cstdint>
#include "board/board.h"
#include "util/bitops.h"

#include "util/assert.h"

bool validBitIndex(std::size_t bit_index) {
    return (bit_index < util::NUM_BITOP_BITS) && (bit_index >= 0);
}

void util::setBit(BitOpType* bits, std::size_t bit_index, bool bit) {
    ASSERT(validBitIndex(bit_index), "index: " + std::to_string(bit_index));
    std::size_t shift_count = (util::NUM_BITOP_BITS - bit_index - 1);
    BitOpType mask = ~((BitOpType)1 << shift_count);
    *bits &= mask;
    *bits |= (static_cast<BitOpType>(bit) << shift_count);
}

bool util::getBit(const BitOpType bits, size_t bit_index) {
    ASSERT(validBitIndex(bit_index), "index: " + std::to_string(bit_index));
    std::size_t shift_count = (util::NUM_BITOP_BITS - bit_index - 1);
    BitOpType mask = (BitOpType)1 << shift_count;
    BitOpType bitVal = (bits & mask) >> shift_count;
    ASSERT((bitVal == 0) || (bitVal == 1), "bitVal: " + std::to_string(bitVal));
    return static_cast<bool>(bitVal);
}

size_t util::popHighestBit(BitOpType* bits) {
    ASSERT(*bits > 0, "cannot pop bits from 0");
    size_t bit_index = __builtin_clzl(static_cast<unsigned long>(*bits));
    setBit(bits, bit_index, 0);
    return bit_index;
}
