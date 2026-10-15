#pragma once

namespace sigproc {

/// Threshold with two levels, so a noisy value near one limit does not flip the output.
/// Turns on when the input goes above `high`. Turns off when it goes below `low`.
/// In between, the output keeps its last state. If low > high, the two are swapped.
template <typename T>
class Hysteresis {
public:
    Hysteresis(T low, T high, bool initial = false) noexcept
        : low_(low <= high ? low : high), high_(low <= high ? high : low), state_(initial) {}

    bool update(T input) noexcept {
        if (state_ && input < low_) {
            state_ = false;
        } else if (!state_ && input > high_) {
            state_ = true;
        }
        return state_;
    }

    [[nodiscard]] bool value() const noexcept { return state_; }

    void reset(bool initial = false) noexcept { state_ = initial; }

private:
    T low_;
    T high_;
    bool state_;
};

}  // namespace sigproc
