/**
 * @file Processor.h
 * @author Gustice
 * @brief Effect-Processor-Class for processing effect scenarios and switching actions
 */
#pragma once

#include "Color.h"
#include "StateMachine.h"
#include <stdint.h>
#include <vector>

namespace Effect {

/**
 * @brief Processor Class
 * @details Maintains State-Machines, processes effect-switching, and invokes generation
 *  of raw data stream for LEDs.
 *      \li If effect scenarios are changed a cross-fading is executed
 *      \li Raw-data-stream for each Color object is generated
 */
class Processor {
  public:
    /**
     * @brief Constructor
     * @param targetCount Count of pixels
     * @param fadeSteps Steps count for cross fade
     */
    Processor(uint8_t targetCount, uint8_t const fadeSteps);

    /**
     * @brief Set next effect sequence
     * @param sequence Reference to sequence that has to be processed next
     * @param sColor Start color. Use noColor to start with default color
     * @param intens Idle intensity for effect. Use NULL to start with default intensity
     */
    void SetEffect(const Macro &sequence, Color const *sColor = noColor,
                   uint8_t intens = gu8_idleIntensity);

    /**
     * @brief Execute step of effect processor
     * @return Reference to color result
     */
    Color const *Tick(void);

  private:
    /// @details Target count
    uint8_t _colorSize;
    /// @details Defines length of cross-fading
    uint8_t _fadeSteps;
    /// @details Current count of cross-fading effect
    uint8_t _fadingCnt;

    /// @details Current color
    Color _pColor;
    /// @details Last applied color (used to cross-fade in intermediate states)
    Color _pColorOld;

    /// @details Reference to current effect instance
    StateMachine *_EffPV;
    /// @details Reference to current last effect
    StateMachine *_EffPV_old;

    /// @brief Common parameter set for both Macro-processors
    MachineParameter_t _parameter;

    /// @details Effect instance 1
    StateMachine _effPV1;
    /// @details Effect instance 2
    StateMachine _effPV2;

    /**
     * @brief Cross-fade current and last effect
     * @param k scaling factor for current cross-fade
     */
    void crossFadeColors(uint8_t k);
};

/**
 * @brief Processor Class
 * @details Maintains State-Machines, processes effect-switching, and invokes generation
 *  of raw data stream for LEDs.
 *      \li If effect scenarios are changed a cross-fading is executed
 *      \li Raw-data-stream for each Color object is generated
 */
template <unsigned cnt> class ProcessorSeries {
  public:
    const unsigned MachineCount = cnt;

    /**
     * @brief Constructor
     * @param targetCount Count of pixels
     * @param fadeSteps Steps count for cross fade
     */
    ProcessorSeries(uint8_t targetCount, const uint8_t fadeSteps)
        : _pColor(), _pColorOld(), _parameter{0x20, 0x20, 0, targetCount} {

        for (size_t i = 0; i < MachineCount; i++) {
            _effPV1[i] = StateMachine(_parameter);
            _effPV2[i] = StateMachine(_parameter);
        }

        _EffPV = _effPV1;
        _EffPV_old = _effPV2;

        for (size_t i = 0; i < MachineCount; i++) {
            _EffPV[i].SetEffect(macDark, noColor);
        }

        _fadeSteps = fadeSteps;
    }

    /**
     * @brief Set next effect sequence
     * @param sequence Reference to sequence that has to be processed next
     * @param sColor Start color. Use noColor to start with default color
     * @param intens Idle intensity for effect. Use NULL to start with default intensity
     */
    void SetEffect(const std::vector<Macro> &sequence, const Color *sColor = noColor,
                   uint8_t intens = gu8_idleIntensity) {
        _fadingCnt = _fadeSteps;

        std::swap(_EffPV, _EffPV_old);

        for (size_t i = 0; i < MachineCount; i++) {
            _EffPV[i].SetEffect(sequence[i], sColor, &intens, 0);
        }
    }

    /**
     * @brief Execute step of effect processor
     * @return Reference to color result
     */
    Color const *Tick(void) {
        uint8_t k;

        for (size_t i = 0; i < MachineCount; i++) {
            _pColor[i] = _EffPV[i].Tick();
        }

        if (_fadingCnt > 0) { // always process soft cross dissolve between different macros
            _fadingCnt--;
            k = (uint16_t)_fadingCnt * 0xFF / _fadeSteps;

            _pColorOld = _EffPV_old->Tick();
            crossFadeColors(k);
        }

        return &_pColor;
    }

  private:
    /// @details Defines length of cross-fading
    uint8_t _fadeSteps;
    /// @details Current count of cross-fading effect
    uint8_t _fadingCnt;

    /// @details Current color
    Color _pColor[cnt];
    /// @details Last applied color (used to cross-fade in intermediate states)
    Color _pColorOld[cnt];

    /// @details Reference to current effect instance
    StateMachine *_EffPV;
    /// @details Reference to current last effect
    StateMachine *_EffPV_old;

    /// @brief Common parameter set for both Macro-processors
    MachineParameter_t _parameter;

    /// @details Effect instance 1
    StateMachine _effPV1[cnt];
    /// @details Effect instance 2
    StateMachine _effPV2[cnt];

    /**
     * @brief Cross-fade current and last effect
     * @param k scaling factor for current cross-fade
     * @return Color* Reference to color result
     */
    void crossFadeColors(uint8_t k) {

        auto temp = _pColorOld * k;
        for (size_t i = 0; i < MachineCount; i++) {
            _pColor[i] = _pColor[i] * (0xFF - k);
            _pColor[i] = _pColor[i] + temp;
        }
    }
};

} // namespace Effect
