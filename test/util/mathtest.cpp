#include "util/math.h"

#include <cstdint>
#include <cstdio>
#include <unordered_map>
#include <iostream>

#include "gtest/gtest.h"

using namespace math;

TEST(MathTest, IsPow2Test) {
    std::unordered_map<std::size_t, bool> test_map = {
           // value                              expected
            { static_cast<size_t>(1) << 63,      true },
            { static_cast<size_t>(1) << 28,      true },
            { 1,                                 true },
            { 3,                                 false }
    };

    for (auto pair : test_map) {
        ASSERT_EQ(pair.second, isPow2(pair.first)) << "value: " << pair.first;
    }
}

TEST(MathTest, Log2CeilTest) {
    std::unordered_map<std::size_t, size_t> test_map = {
           // value                              expected
            { static_cast<size_t>(1) << 63,      63 },
            { static_cast<size_t>(1) << 28,      28 },
            { 1,                                 0 },
            { 3,                                 2 }
    };

    for (auto pair : test_map) {
        ASSERT_EQ(pair.second, log2Ceil(pair.first)) << "value: " << pair.first;
    }
}
