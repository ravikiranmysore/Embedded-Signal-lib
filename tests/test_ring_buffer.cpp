#include <gtest/gtest.h>

#include "sigproc/ring_buffer.hpp"

using sigproc::RingBuffer;

TEST(RingBuffer, StartsEmpty) {
    RingBuffer<int, 3> buf;
    EXPECT_TRUE(buf.empty());
    EXPECT_FALSE(buf.full());
    EXPECT_EQ(buf.size(), 0U);
    EXPECT_EQ((RingBuffer<int, 3>::capacity()), 3U);
}

TEST(RingBuffer, GrowsUntilFull) {
    RingBuffer<int, 3> buf;
    buf.push(1);
    buf.push(2);
    EXPECT_EQ(buf.size(), 2U);
    EXPECT_FALSE(buf.full());
    buf.push(3);
    EXPECT_TRUE(buf.full());
    EXPECT_FALSE(buf.empty());
}

TEST(RingBuffer, IndexZeroIsOldest) {
    RingBuffer<int, 3> buf;
    buf.push(10);
    buf.push(20);
    EXPECT_EQ(buf[0], 10);
    EXPECT_EQ(buf[1], 20);
    EXPECT_EQ(buf.front(), 10);
    EXPECT_EQ(buf.back(), 20);
}

TEST(RingBuffer, OverwritesOldestWhenFull) {
    RingBuffer<int, 3> buf;
    for (int i = 1; i <= 5; ++i) {
        buf.push(i);
    }
    EXPECT_EQ(buf.size(), 3U);
    EXPECT_EQ(buf[0], 3);
    EXPECT_EQ(buf[1], 4);
    EXPECT_EQ(buf[2], 5);
}

TEST(RingBuffer, KeepsOrderAfterManyWraps) {
    RingBuffer<int, 4> buf;
    for (int i = 0; i < 1000; ++i) {
        buf.push(i);
    }
    EXPECT_EQ(buf.front(), 996);
    EXPECT_EQ(buf.back(), 999);
}

TEST(RingBuffer, ClearEmptiesTheBuffer) {
    RingBuffer<int, 3> buf;
    buf.push(1);
    buf.push(2);
    buf.clear();
    EXPECT_TRUE(buf.empty());
    buf.push(7);
    EXPECT_EQ(buf.front(), 7);
    EXPECT_EQ(buf.size(), 1U);
}

TEST(RingBuffer, CapacityOfOneWorks) {
    RingBuffer<int, 1> buf;
    buf.push(1);
    buf.push(2);
    EXPECT_EQ(buf.size(), 1U);
    EXPECT_EQ(buf.front(), 2);
    EXPECT_EQ(buf.back(), 2);
}
