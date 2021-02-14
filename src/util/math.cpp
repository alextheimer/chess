#include <cassert>
#include "util/math.h"

static const std::size_t NUM_SIZE_T_BITS = sizeof(std::size_t) * 8;

bool math::isPow2(std::size_t val) {
    assert(val >= 0);
    return (1 == __builtin_popcountl(val));
}

std::size_t math::log2Ceil(std::size_t val) {
    int num_lead_zeros = __builtin_clzl(val);
    std::size_t result = NUM_SIZE_T_BITS - num_lead_zeros;
    if (isPow2(val)) {
        result -= 1;
    }
    return result;
}
