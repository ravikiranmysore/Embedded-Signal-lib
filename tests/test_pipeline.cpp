#include <gtest/gtest.h>

#include <sigproc/sigproc.hpp>

// Uses the umbrella header and chains two blocks, the way real code would.

TEST(Pipeline, MedianThenHysteresisIgnoresASpikeButCatchesARealRise) {
    sigproc::MedianFilter<double, 5> median;
    sigproc::Hysteresis<double> alarm(60.0, 80.0);

    bool alarmSeen = false;
    for (int i = 0; i < 20; ++i) {
        const double raw = (i == 10) ? 500.0 : 50.0;  // one bad reading
        alarmSeen = alarm.update(median.update(raw)) || alarmSeen;
    }
    EXPECT_FALSE(alarmSeen);

    bool alarmNow = false;
    for (int i = 0; i < 10; ++i) {
        alarmNow = alarm.update(median.update(100.0));  // a real, lasting rise
    }
    EXPECT_TRUE(alarmNow);
}

TEST(Pipeline, DebouncedSwitchFeedsAMovingAverage) {
    sigproc::Debouncer button(2);
    sigproc::MovingAverage<double, 4> dutyCycle;

    const bool noisy[] = {false, true, false, true, true, true, true, true};
    double duty = 0.0;
    for (bool raw : noisy) {
        duty = dutyCycle.update(button.update(raw) ? 1.0 : 0.0);
    }
    EXPECT_DOUBLE_EQ(duty, 1.0);
}
