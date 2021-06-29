// Copyright 2021 Alex Theimer

#include <cstdint>
#include <cstdio>
#include <unordered_map>
#include <iostream>

#include "util/math.h"
#include "gtest/gtest.h"

/*
~~~ Test Partitions ~~~
isPow2
    val: highest-order bit at index 0, at index 63, elsewhere
    val: multiple bits, single bit
log2Ceil
    val: highest-order bit at index 0, at index 63, elsewhere
    val: multiple bits, single bit
*/

/*
Covers:
    isPow2
        val: highest-order bit at index 0, at index 63, elsewhere
        val: multiple bits, single bit

*/
TEST(MathTest, IsPow2Test) {
    std::unordered_map<std::size_t, bool> test_map = {
           // value                              expected
            { static_cast<size_t>(1) << 63,      true },
            { static_cast<size_t>(1) << 28,      true },
            { 1,                                 true },
            { 3,                                 false },
            { 0x8000000000000001,                false }
    };

    for (auto pair : test_map) {
        ASSERT_EQ(pair.second, util::isPow2(pair.first))
                << "value: " << pair.first;
    }
}

/*
Covers:
    log2Ceil
        val: highest-order bit at index 0, at index 63, elsewhere
        val: multiple bits, single bit
*/
TEST(MathTest, Log2CeilTest) {
    std::unordered_map<std::size_t, size_t> test_map = {
           // value                              expected
            { static_cast<size_t>(1) << 63,      63 },
            { static_cast<size_t>(1) << 28,      28 },
            { 1,                                 0 },
            { 3,                                 2 },
            { 0x8000000000000001,                64 }
    };

    for (auto pair : test_map) {
        ASSERT_EQ(pair.second, util::log2Ceil(pair.first))
                << "value: " << pair.first;
    }
}
