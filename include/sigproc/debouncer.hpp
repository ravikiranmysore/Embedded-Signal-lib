#pragma once

#include <cstdint>

namespace sigproc {

/// Clean up a noisy on/off input, such as a switch contact.
/// The output changes only after the raw input has disagreed with it
/// for `samples` calls in a row. A threshold of 0 acts like 1.
class Debouncer {
public:
    [[nodiscard]] explicit Debouncer(std::uint32_t samples, bool initial = false) noexcept
        : threshold_(samples == 0 ? 1U : samples), state_(initial) {}

    bool update(bool raw) noexcept {
        if (raw == state_) {
            counter_ = 0;
        } else if (++counter_ >= threshold_) {
            state_ = raw;
            counter_ = 0;
        }
        return state_;
    }

    [[nodiscard]] bool value() const noexcept { return state_; }

    void reset(bool initial = false) noexcept {
        state_ = initial;
        counter_ = 0;
    }

private:
    std::uint32_t threshold_;
    std::uint32_t counter_ = 0;
    bool state_;
};

}  // namespace sigproc
