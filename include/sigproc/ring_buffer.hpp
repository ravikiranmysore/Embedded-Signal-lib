#pragma once

#include <array>
#include <cstddef>

namespace sigproc {

/// Fixed-size circular buffer. No heap use. When full, the oldest value is overwritten.
/// Index 0 is always the oldest value.
template <typename T, std::size_t N>
class RingBuffer {
    static_assert(N > 0, "RingBuffer capacity must be greater than zero");

public:
    using value_type = T;

    [[nodiscard]] static constexpr std::size_t capacity() noexcept { return N; }
    [[nodiscard]] std::size_t size() const noexcept { return count_; }
    [[nodiscard]] bool empty() const noexcept { return count_ == 0; }
    [[nodiscard]] bool full() const noexcept { return count_ == N; }

    void push(const T& value) noexcept {
        data_[head_] = value;
        head_ = (head_ + 1) % N;
        if (count_ < N) {
            ++count_;
        }
    }

    /// Precondition: index < size(). Index 0 is the oldest value.
    const T& operator[](std::size_t index) const noexcept { return data_[(start() + index) % N]; }

    /// Precondition: not empty.
    [[nodiscard]] const T& front() const noexcept { return (*this)[0]; }
    /// Precondition: not empty.
    [[nodiscard]] const T& back() const noexcept { return (*this)[count_ - 1]; }

    void clear() noexcept {
        head_ = 0;
        count_ = 0;
    }

private:
    [[nodiscard]] std::size_t start() const noexcept { return (head_ + N - count_) % N; }

    std::array<T, N> data_{};
    std::size_t head_ = 0;
    std::size_t count_ = 0;
};

}  // namespace sigproc
