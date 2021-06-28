#include "util/math.h"

#include "util/assert.h"

// The number of bits in std::size_t.
static constexpr std::size_t NUM_SIZE_T_BITS = sizeof(std::size_t) * 8;

bool util::isPow2(std::size_t val) {
    ASSERT(val >= 0, "val: " + std::to_string(val));  // just in case param type ever changes
    return (1 == __builtin_popcountl(val));
}

std::size_t util::log2Ceil(std::size_t val) {
    ASSERT(val > 0, "val: " + std::to_string(val));  // undefined for val == 0
    int num_lead_zeros = __builtin_clzl(val);
    std::size_t result = NUM_SIZE_T_BITS - num_lead_zeros;
    if (isPow2(val)) {
        // Suppose val == 1. Then num_lead_zeros == 63, and result == 1.
        // Hence `result` overcounts by one unless we check for this condition.
        result -= 1;
    }
    return result;
}
