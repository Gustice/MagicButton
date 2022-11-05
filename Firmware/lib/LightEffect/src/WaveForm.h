/**
 * @file WaveForm.h
 * @author Gustice
 * @brief C++ encapsulated waveform definition
 */

#pragma once

#include "EffectWaveforms.h"
#include <stdint.h>

namespace Effect {

/**
 * @brief Waveform definition based on c-definitions
 */
struct WaveForm_t {
    /// @brief Reference to waveform
    const uint8_t *const pWave;
    /// @brief Length of waveform
    const unsigned Length;
};

/// @brief Empty waveform with defined pointer to meaningless data to avoid problems
extern const WaveForm_t EmptyWave;

} // namespace Effect
