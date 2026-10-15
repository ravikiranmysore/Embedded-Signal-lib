#include <gtest/gtest.h>

#include "sigproc/median_filter.hpp"

using sigproc::MedianFilter;

TEST(MedianFilter, UsesSamplesSeenSoFarDuringWarmup) {
    MedianFilter<int, 5> med;
    EXPECT_EQ(med.update(5), 5);
    EXPECT_EQ(med.update(1), 1);  // [1, 5], lower middle
    EXPECT_EQ(med.update(9), 5);  // [1, 5, 9]
    EXPECT_FALSE(med.ready());
}

TEST(MedianFilter, RemovesASingleSpike) {
    MedianFilter<int, 5> med;
    int out = 0;
    for (int x : {1, 1, 1, 100, 1, 1, 1}) {
        out = med.update(x);
        EXPECT_EQ(out, 1);
    }
    EXPECT_TRUE(med.ready());
}

TEST(MedianFilter, KeepsASharpEdge) {
    MedianFilter<int, 5> med;
    for (int i = 0; i < 5; ++i) {
        med.update(0);
    }
    EXPECT_EQ(med.update(10), 0);
    EXPECT_EQ(med.update(10), 0);
    EXPECT_EQ(med.update(10), 10);  // three of five samples are now 10
}

TEST(MedianFilter, WorksWithDoubles) {
    MedianFilter<double, 3> med;
    med.update(1.5);
    med.update(9.5);
    EXPECT_DOUBLE_EQ(med.update(2.5), 2.5);
}

TEST(MedianFilter, ResetClearsHistory) {
    MedianFilter<int, 3> med;
    med.update(100);
    med.update(100);
    med.update(100);
    med.reset();
    EXPECT_FALSE(med.ready());
    EXPECT_EQ(med.update(4), 4);
}

TEST(MedianFilter, WindowOfOneReturnsInput) {
    MedianFilter<int, 1> med;
    EXPECT_EQ(med.update(3), 3);
    EXPECT_EQ(med.update(8), 8);
}
