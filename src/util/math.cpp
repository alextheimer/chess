#include <cassert>
#include "util/math.h"

static const size_t NUM_SIZE_T_BITS = sizeof(size_t) * 8;

bool math::isPow2(size_t val) {
    assert(val >= 0);
    return (1 == __builtin_popcountl(val));
}

size_t math::log2Ceil(size_t val) {
    assert(isPow2(val));
    int num_lead_zeros = __builtin_clzl(val);
    size_t result = NUM_SIZE_T_BITS - num_lead_zeros;
    if (isPow2(val)) {
        result -= 1;
    }
    return result;
}