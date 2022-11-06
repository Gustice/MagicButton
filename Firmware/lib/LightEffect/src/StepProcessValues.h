/**
 * @file StepProcessValues.h
 * @author Gustice
 * @brief Setup-Structure for Effect-State-Machine
 */
#pragma once

#include "Color.h"
#include "WaveForm.h"
#include <stdint.h>

namespace Effect {

/**
 * @brief Collection of process values to perform an EffectStep
 */
struct StepProcessValues_t {
    /// @brief Reference to mutable color-container (output variable)
    Color *color;

    /// @brief Reference of current color
    const Color &pCurentColor;

    /// @brief Number of color entries
    uint8_t ColorLen;

    /// @brief Idle value for intensity
    uint8_t IdleIntensity;

    /// @brief Full value for intensity
    uint8_t FullIntensity;

    /// @brief Dynamic range for effects
    uint8_t DynamicRange;

    /// @brief Index in current waveform
    uint16_t waveIdx;

    /// @brief Reference to processed waveform
    const WaveForm_t *const pWave;
};

/**
 * @brief Effect implementation for State-Machine
 * @details State-Machine invokes referred effect-implementation during processing
 */
typedef void (*EffectImplementation)(StepProcessValues_t &pcsParam);

} // namespace Effect
