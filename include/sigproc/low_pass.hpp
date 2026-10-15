#pragma once

#include <type_traits>

namespace sigproc {

/// First-order low-pass filter (exponential smoothing):  y += alpha * (x - y)
/// The first sample sets the starting output, so there is no ramp-up from zero.
template <typename T>
class LowPass {
    static_assert(std::is_floating_point_v<T>, "LowPass needs a floating point type");

public:
    /// alpha is clamped to 0..1. Small alpha means heavy smoothing. 1 means no filtering.
    [[nodiscard]] explicit LowPass(T alpha) noexcept : alpha_(clamp01(alpha)) {}

    /// Build a filter from a cutoff frequency and a sample rate (both in Hz).
    /// If either value is zero or negative, alpha becomes 0 and the output
    /// holds the first sample.
    [[nodiscard]] static LowPass fromCutoff(T cutoffHz, T sampleRateHz) noexcept {
        if (cutoffHz <= T{0} || sampleRateHz <= T{0}) {
            return LowPass(T{0});
        }
        constexpr T kPi = static_cast<T>(3.14159265358979323846);
        const T dt = T{1} / sampleRateHz;
        const T rc = T{1} / (T{2} * kPi * cutoffHz);
        return LowPass(dt / (rc + dt));
    }

    T update(T sample) noexcept {
        if (!primed_) {
            output_ = sample;
            primed_ = true;
        } else {
            output_ += alpha_ * (sample - output_);
        }
        return output_;
    }

    [[nodiscard]] T value() const noexcept { return output_; }
    [[nodiscard]] T alpha() const noexcept { return alpha_; }

    void reset() noexcept {
        output_ = T{0};
        primed_ = false;
    }

private:
    [[nodiscard]] static T clamp01(T x) noexcept {
        if (x < T{0}) {
            return T{0};
        }
        if (x > T{1}) {
            return T{1};
        }
        return x;
    }

    T alpha_;
    T output_{0};
    bool primed_ = false;
};

}  // namespace sigproc
