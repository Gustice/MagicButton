/**
 * @file ExactIndex.h
 * @author Gustice
 * @brief Index-Class with fix-point arithmetic
 */
#pragma once

#include <algorithm>
#include <stdint.h>

/**
 * @brief Index-Class with fix-point arithmetic for exact stepping though data-points
 * @details Allows stepping through data-points by quotient step-sizes
 */
class ExactIndex {
  public:
    /// @brief Construct Index-object according to number of data points and steps to be taken
    /// @param length
    /// @param steps
    ExactIndex(unsigned length, uint16_t steps) : Length(std::max(1u, length)), Steps(steps) {
        _delta = Length << 8;
        _delta = _delta / steps;
        _index = unsigned(0u - _delta);
    }

    /// @brief Get current index-position as floor value to never pass upper bound
    uint16_t GetIdx(void) { return ((_index) >> 8); };

    /// @brief Move index by quotient step forward
    ExactIndex &operator++() {
        _index = _index + _delta;
        return *this;
    }

    /// @brief Move index by quotient step backward
    ExactIndex &operator--() {
        _index = _index - _delta;
        return *this;
    }

    /// @brief Get current index-position as floor value to never pass upper bound
    operator int() const { return ((_index) >> 8); }

    /// @brief Get size of data set (number of data points)
    unsigned GetLength() { return Length; }
    /// @brief Get number of steps that are to be taken though data set
    uint16_t GetSteps() { return Steps; }

  private:
    /// @brief Number of data points
    unsigned Length;
    /// @brief Steps to be taken through data-set
    uint16_t Steps;

    /// @brief Current index in fixed-point representation
    unsigned _index = 0;

    /// @brief Step-with in fixed-point representation
    unsigned _delta;
};