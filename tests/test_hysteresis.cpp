#include <gtest/gtest.h>

#include "sigproc/hysteresis.hpp"

using sigproc::Hysteresis;

TEST(Hysteresis, TurnsOnAboveHighAndOffBelowLow) {
    Hysteresis<int> h(10, 20);
    EXPECT_FALSE(h.update(15));
    EXPECT_FALSE(h.update(20));  // not above 20
    EXPECT_TRUE(h.update(21));
    EXPECT_TRUE(h.update(15));  // inside the band, keeps state
    EXPECT_TRUE(h.update(10));  // not below 10
    EXPECT_FALSE(h.update(9));
}

TEST(Hysteresis, SwapsReversedLimits) {
    Hysteresis<double> h(20.0, 10.0);
    EXPECT_TRUE(h.update(25.0));
    EXPECT_TRUE(h.update(15.0));
    EXPECT_FALSE(h.update(5.0));
}

TEST(Hysteresis, CanStartOn) {
    Hysteresis<int> h(10, 20, true);
    EXPECT_TRUE(h.update(15));
    EXPECT_FALSE(h.update(9));
}

TEST(Hysteresis, DoesNotChatterNearOneLimit) {
    Hysteresis<double> h(15.0, 25.0);
    for (int i = 0; i < 20; ++i) {
        EXPECT_FALSE(h.update(i % 2 == 0 ? 24.5 : 25.0));
    }
}

TEST(Hysteresis, ResetSetsTheState) {
    Hysteresis<int> h(10, 20);
    h.update(30);
    ASSERT_TRUE(h.value());
    h.reset();
    EXPECT_FALSE(h.value());
    h.reset(true);
    EXPECT_TRUE(h.value());
}
