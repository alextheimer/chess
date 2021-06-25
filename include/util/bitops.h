#ifndef UTIL_BITOPS_H_
#define UTIL_BITOPS_H_

#include <cstdint>

namespace util {

// This is the datatype we perform bitops on
// (standardized to facilitate use of intrinsics).
typedef uint64_t BitOpType;
const std::size_t NUM_BITOP_BITS = 8 * sizeof(BitOpType);

void setBit(BitOpType* bits, std::size_t bit_index, bool bit);
bool getBit(const BitOpType bits, std::size_t bit_index);
std::size_t popHighestBit(BitOpType* bits);

}  // namespace bitops

#endif  // UTIL_BITOPS_H_
