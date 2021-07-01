// Copyright 2021 Alex Theimer

#include <cstdint>
#include "board/board.h"
#include "util/bitops.h"

#include "util/assert.h"

bool isValidBitIndex(std::size_t bit_index) {
    return (bit_index < util::NUM_BITOP_BITS) && (bit_index >= 0);
}

void util::setBit(BitOpType* bits, std::size_t bit_index, bool bit) {
    ASSERT(isValidBitIndex(bit_index), "index: " + std::to_string(bit_index));
    // Courtesy of https://stackoverflow.com/a/47990.
    // This eliminates one bit-shift from a naive implementation and makes
    //     it surprisingly less of a bottleneck.
    *bits ^= (-static_cast<BitOpType>(bit) ^ *bits)
             & (static_cast<BitOpType>(1) << bit_index);
}

bool util::getBit(BitOpType bits, std::size_t bit_index) {
    ASSERT(isValidBitIndex(bit_index), "index: " + std::to_string(bit_index));
    BitOpType bitVal = (bits >> bit_index) & static_cast<BitOpType>(1);
    ASSERT((bitVal == 0) || (bitVal == 1), "bitVal: " + std::to_string(bitVal));
    return static_cast<bool>(bitVal);
}

size_t util::popLowestBit(BitOpType* bits) {
    ASSERT(*bits > 0, "cannot pop bits from 0");
    // mask of all 1's except least-significant bit
    static constexpr BitOpType MASK = ~static_cast<std::size_t>(1);
    std::size_t bit_index =
            __builtin_ctzl(static_cast<std::size_t>(*bits));
    // "extend the zeros" to include the index of the next 1 bit
    *bits &= (MASK << bit_index);
    return bit_index;
}
