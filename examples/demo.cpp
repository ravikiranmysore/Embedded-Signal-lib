// Runs a noisy 1 Hz sine wave with a few bad readings through three filters.
// Prints CSV: time, raw, moving_average, median, low_pass
//
// Build and run:  ./sigproc_demo > demo.csv
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <sigproc/low_pass.hpp>
#include <sigproc/median_filter.hpp>
#include <sigproc/moving_average.hpp>

namespace {

// Small deterministic noise source, so the output is the same on every machine.
double noise(std::uint32_t& state) {
    state = state * 1664525U + 1013904223U;
    return (static_cast<double>(state >> 8) / 16777216.0 - 0.5) * 0.4;  // -0.2 .. +0.2
}

}  // namespace

int main() {
    constexpr double kSampleRate = 100.0;
    constexpr int kSamples = 400;
    constexpr double kTwoPi = 6.28318530717958647692;

    sigproc::MovingAverage<double, 9> average;
    sigproc::MedianFilter<double, 9> median;
    auto lowPass = sigproc::LowPass<double>::fromCutoff(2.0, kSampleRate);

    std::uint32_t seed = 12345U;
    std::printf("t,raw,moving_average,median,low_pass\n");
    for (int i = 0; i < kSamples; ++i) {
        const double t = static_cast<double>(i) / kSampleRate;
        double raw = std::sin(kTwoPi * t) + noise(seed);
        if (i == 120 || i == 121 || i == 250 || i == 310) {
            raw += 3.0;  // bad readings
        }
        std::printf("%.2f,%.4f,%.4f,%.4f,%.4f\n", t, raw, average.update(raw), median.update(raw),
                    lowPass.update(raw));
    }
    return 0;
}
