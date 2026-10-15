#pragma once

#include <cstddef>
#include <type_traits>

#include "sigproc/ring_buffer.hpp"

namespace sigproc {

/// Moving average over the last N samples.
/// Until N samples have arrived, it averages the samples seen so far.
/// A running sum keeps each update cheap. The sum is rebuilt from the buffer
/// every N updates, so rounding error cannot build up over time.
template <typename T, std::size_t N>
class MovingAverage {
    static_assert(N > 0, "Window size must be greater than zero");
    static_assert(std::is_floating_point_v<T>, "MovingAverage needs a floating point type");

public:
    T update(T sample) noexcept {
        if (buffer_.full()) {
            sum_ -= buffer_.front();
        }
        buffer_.push(sample);
        sum_ += sample;

        if (++sinceResync_ >= N) {
            resync();
        }
        return value();
    }

    /// Current average. Zero before the first sample.
    [[nodiscard]] T value() const noexcept {
        return buffer_.empty() ? T{0} : sum_ / static_cast<T>(buffer_.size());
    }

    /// True once the window is full.
    [[nodiscard]] bool ready() const noexcept { return buffer_.full(); }

    void reset() noexcept {
        buffer_.clear();
        sum_ = T{0};
        sinceResync_ = 0;
    }

private:
    void resync() noexcept {
        T sum{0};
        for (std::size_t i = 0; i < buffer_.size(); ++i) {
            sum += buffer_[i];
        }
        sum_ = sum;
        sinceResync_ = 0;
    }

    RingBuffer<T, N> buffer_{};
    T sum_{0};
    std::size_t sinceResync_ = 0;
};

}  // namespace sigproc
