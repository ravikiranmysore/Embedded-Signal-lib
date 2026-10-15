#include <gtest/gtest.h>

#include "sigproc/debouncer.hpp"

using sigproc::Debouncer;

TEST(Debouncer, IgnoresAShortGlitch) {
    Debouncer deb(3);
    EXPECT_FALSE(deb.update(true));
    EXPECT_FALSE(deb.update(true));
    EXPECT_FALSE(deb.update(false));
}

TEST(Debouncer, SwitchesAfterEnoughConsecutiveSamples) {
    Debouncer deb(3);
    EXPECT_FALSE(deb.update(true));
    EXPECT_FALSE(deb.update(true));
    EXPECT_TRUE(deb.update(true));
    EXPECT_TRUE(deb.value());
}

TEST(Debouncer, CountRestartsWhenTheInputAgreesAgain) {
    Debouncer deb(3);
    deb.update(true);
    deb.update(true);
    deb.update(false);  // breaks the run
    EXPECT_FALSE(deb.update(true));
    EXPECT_FALSE(deb.update(true));
    EXPECT_TRUE(deb.update(true));
}

TEST(Debouncer, FallingEdgeIsDebouncedToo) {
    Debouncer deb(3, true);
    EXPECT_TRUE(deb.update(false));
    EXPECT_TRUE(deb.update(false));
    EXPECT_FALSE(deb.update(false));
}

TEST(Debouncer, ThresholdZeroActsLikeOne) {
    Debouncer deb(0);
    EXPECT_TRUE(deb.update(true));
    EXPECT_FALSE(deb.update(false));
}

TEST(Debouncer, ResetSetsTheState) {
    Debouncer deb(2);
    deb.update(true);
    deb.update(true);
    ASSERT_TRUE(deb.value());
    deb.reset();
    EXPECT_FALSE(deb.value());
    deb.reset(true);
    EXPECT_TRUE(deb.value());
}
