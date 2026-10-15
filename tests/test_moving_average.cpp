#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <cstddef>

#include "sigproc/moving_average.hpp"

using sigproc::MovingAverage;

TEST(MovingAverage, IsZeroBeforeFirstSample) {
    MovingAverage<double, 4> avg;
    EXPECT_DOUBLE_EQ(avg.value(), 0.0);
    EXPECT_FALSE(avg.ready());
}

TEST(MovingAverage, AveragesSamplesSeenSoFarDuringWarmup) {
    MovingAverage<double, 4> avg;
    EXPECT_DOUBLE_EQ(avg.update(2.0), 2.0);
    EXPECT_DOUBLE_EQ(avg.update(4.0), 3.0);
    EXPECT_DOUBLE_EQ(avg.update(6.0), 4.0);
    EXPECT_FALSE(avg.ready());
    EXPECT_DOUBLE_EQ(avg.update(8.0), 5.0);
    EXPECT_TRUE(avg.ready());
}

TEST(MovingAverage, SlidesOverTheWindow) {
    MovingAverage<double, 4> avg;
    for (double x : {2.0, 4.0, 6.0, 8.0}) {
        avg.update(x);
    }
    EXPECT_DOUBLE_EQ(avg.update(10.0), 7.0);  // (4 + 6 + 8 + 10) / 4
    EXPECT_DOUBLE_EQ(avg.update(12.0), 9.0);  // (6 + 8 + 10 + 12) / 4
}

TEST(MovingAverage, StepReachesFinalValueAfterWindowLength) {
    MovingAverage<double, 5> avg;
    for (int i = 0; i < 5; ++i) {
        avg.update(0.0);
    }
    double out = 0.0;
    for (int i = 0; i < 5; ++i) {
        out = avg.update(10.0);
    }
    EXPECT_DOUBLE_EQ(out, 10.0);
}

TEST(MovingAverage, ResetClearsHistory) {
    MovingAverage<double, 3> avg;
    avg.update(100.0);
    avg.update(100.0);
    avg.reset();
    EXPECT_DOUBLE_EQ(avg.value(), 0.0);
    EXPECT_DOUBLE_EQ(avg.update(6.0), 6.0);
}

TEST(MovingAverage, WorksWithFloat) {
    MovingAverage<float, 2> avg;
    avg.update(1.0F);
    EXPECT_FLOAT_EQ(avg.update(3.0F), 2.0F);
}

TEST(MovingAverage, StaysAccurateOverAVeryLongRun) {
    constexpr std::size_t kWindow = 8;
    constexpr int kSamples = 200000;
    MovingAverage<float, kWindow> avg;
    double last[kWindow] = {};
    float out = 0.0F;
    for (int i = 0; i < kSamples; ++i) {
        const float x = (i % 7 == 0) ? 1.0e6F : 0.37F * static_cast<float>(i % 5);
        out = avg.update(x);
        last[static_cast<std::size_t>(i) % kWindow] = static_cast<double>(x);
    }
    double reference = 0.0;
    for (double v : last) {
        reference += v;
    }
    reference /= static_cast<double>(kWindow);
    const double tolerance = std::max(1.0e-4 * std::fabs(reference), 1.0e-3);
    EXPECT_NEAR(static_cast<double>(out), reference, tolerance);
}
