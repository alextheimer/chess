#ifndef BITOPS_H_
#define BITOPS_H_

#include <cstdint>

namespace bitops {

	typedef uint64_t BitOpType;
	const std::size_t NUM_BITS = 8 * sizeof(BitOpType);

    void set_bit(BitOpType& bits, std::size_t bit_index, bool bit);
    bool get_bit(BitOpType bits, std::size_t bit_index);
}

#endif  // BITOPS_H_
