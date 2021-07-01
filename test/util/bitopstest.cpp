// Copyright 2021 Alex Theimer

#include <cstdint>
#include <cstdio>
#include <vector>
#include <iostream>
#include <sstream>

#include "util/bitops.h"
#include "gtest/gtest.h"

using util::BitOpType;

/*
~~~ Test Partitions ~~~
getBit
    index: first, last, elsewhere
    expected: 0, 1
setBit
    index: first, last, elsewhere
    original: 0, 1
    set_bit: 0, 1
    original/set_big: same, different
popLowestBit
    result: first, last, elsewhere
    bits (after pop): 0, other
*/

/*
Covers:
    getBit
        index: first, last, elsewhere
        expected: 0, 1
*/
TEST(BitOpsTest, GetBitTest) {
    struct TestSpec {
        BitOpType bits;
        std::size_t index;
        bool expected;
    };

    std::vector<TestSpec> spec_vec = {
           // bits                                  index    expected
            { static_cast<BitOpType>(1) << 63,      63,      true },
            { static_cast<BitOpType>(1) << 63,      0,       false },
            { 1,                                    0,       true },
            { 1,                                    63,      false },
            { 7,                                    1,       true  },
            { 0b101101,                             4,       false }
    };

    for (TestSpec test_spec : spec_vec) {
        ASSERT_EQ(test_spec.expected,
                  util::getBit(test_spec.bits, test_spec.index))
            << "bits: " << test_spec.bits << ", "
            << "index: " << test_spec.index;
    }
}

/*
Covers:
    setBit
        index: first, last, elsewhere
        original: 0, 1
        set_bit: 0, 1
        original/set_big: same, different
*/
TEST(BitOpsTest, SetBitTest) {
    struct TestSpec {
        BitOpType bits;
        std::size_t index;
        bool expected_before;
        bool set_bit;
    };

    // Note: `before` is included only as a sanity check.
    //     get_bit will "confirm" that we're setting
    //     the bit from an expected value.
    std::vector<TestSpec> spec_vec = {
           // bits                                  index    before    set_bit
            { static_cast<BitOpType>(1) << 63,      63,      true,     false },
            { static_cast<BitOpType>(1) << 63,      0,       false,    true },
            { 1,                                    0,       true,     false },
            { 1,                                    63,      false,    true },
            { 7,                                    1,       true,     false },
            { 0b101101,                             4,       false,    true },
            { 7,                                    1,       true,     true },
            { 0b101101,                             4,       false,    false }
    };

    for (TestSpec test_spec : spec_vec) {
        std::stringstream error_msg;
        error_msg << "original bits: " << test_spec.bits << ", "
                  << "index: " << test_spec.index << ", "
                  << "set_bit: " << test_spec.set_bit;
        BitOpType bits = test_spec.bits;
        // sanity check!
        ASSERT_EQ(test_spec.expected_before,
                  util::getBit(bits, test_spec.index)) << error_msg.str();
        util::setBit(&bits, test_spec.index, test_spec.set_bit);
        ASSERT_EQ(test_spec.set_bit,
                  util::getBit(bits, test_spec.index)) << error_msg.str();
    }
}

/*
Covers:
    popLowestBit
        result: first, last, elsewhere
        bits (after pop): 0, other
*/
TEST(BitOpsTest, PopLowestBitTest) {
    struct TestSpec {
        BitOpType bits;
        BitOpType expected_bits;
        std::size_t expected_index;
    };

    std::vector<TestSpec> spec_vec = {
           // bits                               expected_bits   expected_index
            { static_cast<BitOpType>(1) << 63,   0,              63 },
            { 1,                                 0,              0 },
            { 6,                                 4,              1 }
    };

    for (TestSpec test_spec : spec_vec) {
        BitOpType bits = test_spec.bits;
        ASSERT_EQ(test_spec.expected_index, util::popLowestBit(&bits))
            << "bits before: " << test_spec.bits << ", after: " << bits;
        ASSERT_EQ(test_spec.expected_bits, bits);
    }
}
