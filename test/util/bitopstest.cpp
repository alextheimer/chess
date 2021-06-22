#include "util/bitops.h"

#include <cstdint>
#include <cstdio>
#include <vector>
#include <iostream>
#include <sstream>

#include "gtest/gtest.h"

using namespace bitops;

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
            { 1,                                    0,       false }
    };

    for (TestSpec test_spec : spec_vec) {
        ASSERT_EQ(test_spec.expected, getBit(test_spec.bits, test_spec.index))
            << "bits: " << test_spec.bits << ", "
            << "index: " << test_spec.index;
    }
}

TEST(BitOpsTest, SetBitTest) {
    typedef struct TestSpec {
        BitOpType bits;
        std::size_t index;
        bool expected_before;
        bool set_bit;
    } TestSpec;

    std::vector<TestSpec> spec_vec = {
           // bits                                  index    before    set_bit
            { static_cast<BitOpType>(1) << 63,      0,       true,     false },
            { static_cast<BitOpType>(1) << 63,      63,      false,    true },
            { 1,                                    63,      true,     false },
            { 1,                                    0,       false,    true }
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
