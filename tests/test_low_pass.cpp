#include <gtest/gtest.h>

#include <cmath>

#include "sigproc/low_pass.hpp"

using sigproc::LowPass;

TEST(LowPass, FirstSampleSetsTheOutput) {
    LowPass<double> lp(0.1);
    EXPECT_DOUBLE_EQ(lp.update(5.0), 5.0);
}

TEST(LowPass, FollowsTheSmoothingFormula) {
    LowPass<double> lp(0.5);
    lp.update(0.0);
    EXPECT_DOUBLE_EQ(lp.update(8.0), 4.0);
    EXPECT_DOUBLE_EQ(lp.update(8.0), 6.0);
    EXPECT_DOUBLE_EQ(lp.value(), 6.0);
}

TEST(LowPass, ConvergesToAConstantInput) {
    LowPass<double> lp(0.2);
    lp.update(0.0);
    double out = 0.0;
    for (int i = 0; i < 100; ++i) {
        out = lp.update(10.0);
    }
    EXPECT_NEAR(out, 10.0, 1.0e-6);
}

TEST(LowPass, AlphaIsClampedToZeroAndOne) {
    EXPECT_DOUBLE_EQ(LowPass<double>(-1.0).alpha(), 0.0);
    EXPECT_DOUBLE_EQ(LowPass<double>(2.0).alpha(), 1.0);
    EXPECT_DOUBLE_EQ(LowPass<double>(0.25).alpha(), 0.25);
}

TEST(LowPass, AlphaOneIsPassThrough) {
    LowPass<double> lp(1.0);
    lp.update(3.0);
    EXPECT_DOUBLE_EQ(lp.update(7.0), 7.0);
}

TEST(LowPass, AlphaZeroHoldsTheFirstSample) {
    LowPass<double> lp(0.0);
    lp.update(3.0);
    EXPECT_DOUBLE_EQ(lp.update(100.0), 3.0);
}

TEST(LowPass, FromCutoffMatchesTheFormula) {
    const double pi = std::acos(-1.0);
    const double dt = 1.0 / 100.0;
    const double rc = 1.0 / (2.0 * pi * 10.0);
    const auto lp = LowPass<double>::fromCutoff(10.0, 100.0);
    EXPECT_NEAR(lp.alpha(), dt / (rc + dt), 1.0e-12);
}

TEST(LowPass, FromCutoffRejectsBadInput) {
    EXPECT_DOUBLE_EQ(LowPass<double>::fromCutoff(0.0, 100.0).alpha(), 0.0);
    EXPECT_DOUBLE_EQ(LowPass<double>::fromCutoff(-5.0, 100.0).alpha(), 0.0);
    EXPECT_DOUBLE_EQ(LowPass<double>::fromCutoff(10.0, 0.0).alpha(), 0.0);
}

TEST(LowPass, ResetStartsOverFromTheNextSample) {
    LowPass<double> lp(0.5);
    lp.update(100.0);
    lp.reset();
    EXPECT_DOUBLE_EQ(lp.update(2.0), 2.0);
}

TEST(LowPass, StronglyReducesAFastAlternatingSignal) {
    LowPass<double> lp(0.05);
    double out = 0.0;
    for (int i = 0; i < 400; ++i) {
        out = lp.update(i % 2 == 0 ? 1.0 : -1.0);
    }
    EXPECT_LT(std::fabs(out), 0.05);
}
