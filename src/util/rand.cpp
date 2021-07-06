// Copyright 2021 Alex Theimer

#include "util/rand.h"

#include <random>

std::uint64_t util::getRand64() {
    // one random generator per "instance" of this function.
    // TODO(theimer): does this mean two inlined versions of this
    //     function would give the same sequence of values?
    static std::mt19937_64 rand_gen(std::random_device{}());
    return rand_gen();
}
