#include <cassert>
#include "util/math.h"

// The number of bits in size_t.
static const std::size_t NUM_SIZE_T_BITS = sizeof(std::size_t) * 8;

bool math::isPow2(std::size_t val) {
    assert(val >= 0);  // just in case param type ever changes
    return (1 == __builtin_popcountl(val));
}

std::size_t math::log2Ceil(std::size_t val) {
	assert(val > 0);  // undefined for val == 0
    int num_lead_zeros = __builtin_clzl(val);
    std::size_t result = NUM_SIZE_T_BITS - num_lead_zeros;
    if (isPow2(val)) {
    	// Suppose val == 1. Then num_lead_zeros == 63, and result == 1.
    	// Hence `result` overcounts by one unless we check for this condition.
        result -= 1;
    }
    return result;
}
