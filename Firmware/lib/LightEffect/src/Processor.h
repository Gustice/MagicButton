/**
 * @file Processor.h
 * @author Gustice
 * @brief Effect-Processor-Class for processing effect scenarios and switching actions
 */
#pragma once

#include "Color.h"
#include "StateMachine.h"
#include <algorithm>
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
    Processor(uint8_t const fadeSteps)
        : _pColor(), _pColorOld(), _parameter{0x20, 0x20, 0, 1}, _effPV1(_parameter),
          _effPV2(_parameter) {

        _EffPV = &_effPV1;
        _EffPV_old = &_effPV2;
        _EffPV->SetEffect(macDark, noColor);
        _fadeSteps = fadeSteps;
    }

    /**
     * @brief Set next effect sequence
     * @param sequence Reference to sequence that has to be processed next
     * @param sColor Start color. Use noColor to start with default color
     * @param intens Idle intensity for effect. Use NULL to start with default intensity
     */
    void SetEffect(const Macro &sequence, Color const *sColor = noColor,
                   uint8_t intens = gu8_idleIntensity) {
        _fadingCnt = _fadeSteps;

        std::swap(_EffPV, _EffPV_old);
        _EffPV->SetEffect(sequence, sColor, &intens, 0);
    }

    /**
     * @brief Execute step of effect processor
     * @return Reference to color result
     */
    Color const *Tick(void) {
        uint8_t k;
        _EffPV->Tick(&_pColor);

        if (_fadingCnt > 0) { // always process soft cross dissolve between different macros
            _fadingCnt--;
            k = (uint16_t)_fadingCnt * 0xFF / _fadeSteps;

            _EffPV_old->Tick(&_pColorOld);
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
    void crossFadeColors(uint8_t k) {
        Color temp = _pColorOld * k;
        _pColor = _pColor * (0xFF - k);
        _pColor = _pColor + temp;
    }
};

/**
 * @brief Processor Class
 * @details Maintains State-Machines, processes effect-switching, and invokes generation
 *  of raw data stream for LEDs.
 *      \li If effect scenarios are changed a cross-fading is executed
 *      \li Raw-data-stream for each Color object is generated
 */
template <unsigned LedCnt = 1> class MultiProcessor {
  public:
    uint8_t LedCount = LedCnt;

    /**
     * @brief Constructor
     * @param targetCount Count of pixels
     * @param fadeSteps Steps count for cross fade
     */
    MultiProcessor(uint8_t const fadeSteps)
        : _parameter{0x20, 0x20, 0, LedCount}, _effPV1(_parameter), _effPV2(_parameter) {

        _EffPV = &_effPV1;
        _EffPV_old = &_effPV2;
        _EffPV->SetEffect(macDark, &CBlack);
        _fadeSteps = fadeSteps;
    }

    /**
     * @brief Set next effect sequence
     * @param sequence Reference to sequence that has to be processed next
     * @param sColor Start color. Use noColor to start with default color
     * @param intens Idle intensity for effect. Use NULL to start with default intensity
     */
    void SetEffect(const Macro &sequence, Color const *sColor = noColor,
                   uint8_t intens = gu8_idleIntensity) {
        _fadingCnt = _fadeSteps;

        std::swap(_EffPV, _EffPV_old);
        _EffPV->SetEffect(sequence, sColor, &intens, 0);
    }

    /**
     * @brief Execute step of effect processor
     * @return Reference to color result
     */
    Color const *Tick(void) {
        uint8_t k;
        _EffPV->Tick(_pColor);

        if (_fadingCnt > 0) { // always process soft cross dissolve between different macros
            _fadingCnt--;
            k = (uint16_t)_fadingCnt * 0xFF / _fadeSteps;

            _EffPV_old->Tick(_pColorOld);
            crossFadeColors(k);
        }

        return _pColor;
    }

  private:
    /// @details Defines length of cross-fading
    uint8_t _fadeSteps;
    /// @details Current count of cross-fading effect
    uint8_t _fadingCnt;

    /// @details Current color
    Color _pColor[LedCnt];
    /// @details Last applied color (used to cross-fade in intermediate states)
    Color _pColorOld[LedCnt];

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
    void crossFadeColors(uint8_t k) {
        for (size_t i = 0; i < LedCnt; i++) {
            Color temp = _pColorOld[i] * k;
            _pColor[i] = _pColor[i] * (0xFF - k);
            _pColor[i] = _pColor[i] + temp;
        }
    }
};

/**
 * @brief Processor Class
 * @details Maintains State-Machines, processes effect-switching, and invokes generation
 *  of raw data stream for LEDs.
 *      \li If effect scenarios are changed a cross-fading is executed
 *      \li Raw-data-stream for each Color object is generated
 */
template <unsigned LedCnt, unsigned ParCnt> class ProcessorSeries {
  public:
    const unsigned MachineCount = ParCnt;

    /**
     * @brief Constructor
     * @param targetCount Count of pixels
     * @param fadeSteps Steps count for cross fade
     */
    ProcessorSeries(const uint8_t fadeSteps)
        : _pColor(), _pColorOld(), _parameter{0x20, 0x20, 0, LedCnt} {

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
            _EffPV[i].Tick(_pColor);
        }

        if (_fadingCnt > 0) { // always process soft cross dissolve between different macros
            _fadingCnt--;
            k = (uint16_t)_fadingCnt * 0xFF / _fadeSteps;

            _EffPV_old->Tick(_pColorOld);
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
    Color _pColor[ParCnt];
    /// @details Last applied color (used to cross-fade in intermediate states)
    Color _pColorOld[ParCnt];

    /// @details Reference to current effect instance
    StateMachine *_EffPV;
    /// @details Reference to current last effect
    StateMachine *_EffPV_old;

    /// @brief Common parameter set for both Macro-processors
    MachineParameter_t _parameter;

    /// @details Effect instance 1
    StateMachine _effPV1[ParCnt];
    /// @details Effect instance 2
    StateMachine _effPV2[ParCnt];

    /**
     * @brief Cross-fade current and last effect
     * @param k scaling factor for current cross-fade
     * @return Color* Reference to color result
     */
    void crossFadeColors(uint8_t k) {

        Color temp = _pColorOld * k;
        for (size_t i = 0; i < MachineCount; i++) {
            _pColor[i] = _pColor[i] * (0xFF - k);
            _pColor[i] = _pColor[i] + temp;
        }
    }
};

} // namespace Effect
