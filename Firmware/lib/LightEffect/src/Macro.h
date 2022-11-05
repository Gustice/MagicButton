/**
 * @file Macro.h
 * @author Gustice
 * @brief Effect macro and predefined effect macros
 */

#pragma once

#include "Color.h"
#include "StepProcessValues.h"
#include "WaveForm.h"
#include <stdint.h>
#include <vector>

namespace Effect {

/**
 * @brief Definition of Effect-Macro
 */
struct Macro_t {
    /// @brief Effect type
    const EffectImplementation pImplementation;
    /// @brief Effect duration waveforms will be compressed / stretched to fit the duration
    const uint8_t Duration;
    /// @brief Next SubEffect
    const int8_t Next;
    /// @brief waveform reference
    const WaveForm_t *pWave = emptyWave;
    /// @brief Full-Scale-intensity
    const uint8_t FsIntensity;
    /// @brief SubEffect color
    const Color *pColor;
    /// @brief Num of Repeats
    const int8_t Repeats;

    Macro_t(uint8_t duration, int8_t next, EffectImplementation state, uint8_t intensity,
            const Color *pColor = oldColor, int8_t repeats = 0)
        : pImplementation(state), Duration(duration), Next(next), FsIntensity(intensity),
          pColor(pColor), Repeats(repeats) {}

    Macro_t(uint8_t duration, int8_t next, WaveForm_t const *wave, EffectImplementation state,
            uint8_t intensity, const Color *pColor = oldColor, int8_t repeats = 0)
        : pImplementation(state), Duration(duration), Next(next), pWave(wave),
          FsIntensity(intensity), pColor(pColor), Repeats(repeats) {}

  private:
    /// @brief Null-pointer to represent no change in color
    static constexpr Color *pOldColor = nullptr;

    /// @brief Null-pointer to represent no actual waveform-representation
    static constexpr WaveForm_t *pNoWave = nullptr;

    /// @brief Default implemented Waveform with no profile
    static const constexpr WaveForm_t *emptyWave = &EmptyWave;
};
using Macro = std::vector<Macro_t>;

/// @brief Dark - Nothing to see here
extern const Macro macDark;

/// @brief Continuous idle
extern const Macro macIdle;

/// @brief Smooth turn on and then remaining idle
extern const Macro macStartIdle;

/// @brief Smooth turn on to full color
extern const Macro macStartFull;

/// @brief Slow pulse starting from idle intensity. Appears like breathing
extern const Macro macStdPulse;

/// @brief Short pulses starting from idle intensity. Appears nervously.
extern const Macro macNervousPulse;

/// @brief Swipe-Effect (one side to other)
extern const std::vector<Macro> macDualStdWip;

/// @brief Offset-Pulse:: Pulse on top of idle intensity
extern const WaveForm_t OffsetPulseWave;

/// @brief Full-Pulse:: Pulse starting/ending at dark
extern const WaveForm_t FullPulseWave;

/// @brief Init-Slop:: Fading in idle intensity
extern const WaveForm_t InitSlopeWave;

/// @brief Offset-Slope:: Fading intensity to full intensity from idle
extern const WaveForm_t OffsetSlopeWave;

/// @brief Full-Pulse:: Fading from dark to full intensity
extern const WaveForm_t FullSlopeWave;



} // namespace Effect
