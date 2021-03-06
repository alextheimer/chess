// Copyright 2021 Alex Theimer

#ifndef UTIL_BITOPS_H_
#define UTIL_BITOPS_H_

#include <cstdint>

namespace util {

// datatype these functions operate on
typedef uint64_t BitOpType;

// number of bits in BitOpType
constexpr std::size_t NUM_BITOP_BITS = 8 * sizeof(BitOpType);

/*
Modifies a bit ***in-place*** at the specified index.
@param bit_index: 0 is least-significant
*/
void setBit(BitOpType* bits, std::size_t bit_index, bool bit);

/*
@param bit_index: 0 is least-significant.
*/
bool getBit(BitOpType bits, std::size_t bit_index);

/*
Pops the least-significant bit ***in-place*** and returns its index.
@return: index of the popped bit, where 0 is least-significant.
*/
std::size_t popLowestBit(BitOpType* bits);

}  // namespace util

#endif  // UTIL_BITOPS_H_
