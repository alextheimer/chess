#include "util/bitops.h"

#include <cstdint>
#include <cstdio>
#include <vector>
#include <iostream>

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

    char msg_buffer[100];
    for (TestSpec test_spec : spec_vec) {
        sprintf(msg_buffer, "bits: %lu, index: %lu", test_spec.bits, test_spec.index);
        ASSERT_EQ(test_spec.expected, getBit(test_spec.bits, test_spec.index)) << msg_buffer;
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

    char msg_buffer[100];
    for (TestSpec test_spec : spec_vec) {
        BitOpType bits = test_spec.bits;
        sprintf(msg_buffer, "original bits: %lu, index: %lu, set_bit: %b",
                test_spec.bits, test_spec.index, test_spec.set_bit);
        // sanity check
        ASSERT_EQ(test_spec.expected_before, getBit(bits, test_spec.index)) << msg_buffer;
        setBit(&bits, test_spec.index, test_spec.set_bit);
        ASSERT_EQ(test_spec.set_bit, getBit(bits, test_spec.index)) << msg_buffer;
    }
}
