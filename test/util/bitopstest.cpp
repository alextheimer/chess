#include "util/bitops.h"

#include <cstdint>
#include <cstdio>
#include <vector>
#include <iostream>
#include <sstream>

#include "gtest/gtest.h"

using namespace bitops;

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
popHighestBit
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
    typedef struct TestSpec {
        BitOpType bits;
        std::size_t index;
        bool expected;
    } TestSpec;

    std::vector<TestSpec> spec_vec = {
           // bits                                  index    expected
            { static_cast<BitOpType>(1) << 63,      0,       true },
            { static_cast<BitOpType>(1) << 63,      63,      false },
            { 1,                                    63,      true },
            { 1,                                    0,       false },
            { 7,                                    62,      true  },
            { 0b101101,                             59,      false }
    };

    for (TestSpec test_spec : spec_vec) {
        ASSERT_EQ(test_spec.expected, getBit(test_spec.bits, test_spec.index))
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
    typedef struct TestSpec {
        BitOpType bits;
        std::size_t index;
        bool expected_before;
        bool set_bit;
    } TestSpec;

    /*
    Note: `before` is included only as a sanity check.
    get_bit will "confirm" that we're setting the bit from an expected value.
    */
    std::vector<TestSpec> spec_vec = {
           // bits                                  index    before    set_bit
            { static_cast<BitOpType>(1) << 63,      0,       true,     false },
            { static_cast<BitOpType>(1) << 63,      63,      false,    true },
            { 1,                                    63,      true,     false },
            { 1,                                    0,       false,    true },
            { 7,                                    62,      true,     false },
            { 0b101101,                             59,      false,    true },
            { 7,                                    62,      true,     true },
            { 0b101101,                             59,      false,    false }
    };

    for (TestSpec test_spec : spec_vec) {
        std::stringstream error_msg;
        error_msg << "original bits: " << test_spec.bits << ", "
                  << "index: " << test_spec.index << ", "
                  << "set_bit: " << test_spec.set_bit;

        BitOpType bits = test_spec.bits;
        // sanity check
        ASSERT_EQ(test_spec.expected_before, getBit(bits, test_spec.index)) << error_msg.str();
        setBit(&bits, test_spec.index, test_spec.set_bit);
        ASSERT_EQ(test_spec.set_bit, getBit(bits, test_spec.index)) << error_msg.str();
    }
}

/*
Covers:
    popHighestBit
        result: first, last, elsewhere
        bits (after pop): 0, other
*/
TEST(BitOpsTest, PopHighestBitTest) {
    struct TestSpec {
        BitOpType bits;
        BitOpType expected_bits;
        std::size_t expected_index;
    };

    std::vector<TestSpec> spec_vec = {
           // bits                                expected_bits    expected_index
            { static_cast<BitOpType>(1) << 63,    0,               0 },
            { 1,                                  0,               63 },
            { 7,                                  3,               61 }
    };

    for (TestSpec test_spec : spec_vec) {
        BitOpType bits = test_spec.bits;
        ASSERT_EQ(test_spec.expected_index, popHighestBit(&bits))
            << "bits before: " << test_spec.bits << ", after: " << bits;
        ASSERT_EQ(test_spec.expected_bits, bits);
    }
}
