#pragma once

#include <algorithm>
#include <array>
#include <cstddef>

#include "sigproc/ring_buffer.hpp"

namespace sigproc {

/// Median over the last N samples. N must be odd.
/// A median removes single spikes and keeps sharp edges, which an average smears.
/// Until N samples have arrived, it returns the median of the samples seen so far
/// (the lower middle value when the count is even).
template <typename T, std::size_t N>
class MedianFilter {
    static_assert(N > 0, "Window size must be greater than zero");
    static_assert(N % 2 == 1, "MedianFilter window size must be odd");

public:
    T update(T sample) noexcept {
        buffer_.push(sample);

        std::array<T, N> sorted{};
        const std::size_t count = buffer_.size();
        for (std::size_t i = 0; i < count; ++i) {
            sorted[i] = buffer_[i];
        }
        std::sort(sorted.begin(), sorted.begin() + static_cast<std::ptrdiff_t>(count));
        return sorted[(count - 1) / 2];
    }

    [[nodiscard]] bool ready() const noexcept { return buffer_.full(); }

    void reset() noexcept { buffer_.clear(); }

private:
    RingBuffer<T, N> buffer_{};
};

}  // namespace sigproc
