#include "util/rand.h"

#include <random>

static std::mt19937_64 RAND_GEN(std::random_device{}());

std::uint64_t util::getRand64() {
    return RAND_GEN();
}
