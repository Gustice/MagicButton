/**
 * @file EffectStateMachine.h
 * @author Gustice
 * @brief Effect-State-Machine-Class for processing Effect Macros
 */
#pragma once

#include "Color.h"
#include "ExactIndex.h"
#include "Macro.h"
#include "StepProcessValues.h"
#include <stdint.h>

namespace Effect {

/**
 * @brief Collection of object configuration at startup
 */
struct MachineParameter_t {
    /// @brief Idle intensity as background color
    uint8_t idleIntens;
    /// @brief dynamic range as variation around background color
    uint8_t dynamicRange;
    /// @brief Fade steps for color changes
    uint8_t fadeSteps;
    /// @brief Count of target LEDs
    uint8_t targetCount;
    /// @brief Cross-fade while switching to next SequenceElement
    uint8_t stepCrossFade;
};

/**
 * @brief Effect-State-Machine-Class
 * @details Processes effect macros subsequently in following manner:
 *  \li Index-incrementing each Tick
 *      \li ticks until duration \ref EffMacro is finished
 *      \li each tick the index incremented by templateLength/duration
 *  \li Repeats-processing
 *      \li Each repeat the tick and the index are set back to start
 *      \li Switches to next Macro enty
 *      \li Next entry is given by 'next' index
 *      \li Color changes can be executed optionally
 */
class StateMachine {
  public:
    /**
     * @brief Construct a new Effect State Machine object
     * @param templateLength Setup for state-machine
     */
    StateMachine(MachineParameter_t &parameter);

    /**
     * @brief Sets the effect state machine to process given effect macro
     * @param sequence reference to sequence of macros
     * @param startColor Start color. Use noColor to invoke with default color
     * @param initialDelay Number of steps that are waited before effect is started
     */
    void SetEffect(const Macro &sequence, Color const *startColor = noColor,
                   uint8_t initialDelay = 0);

    /**
     * @brief Sets the effect state machine to process given effect macro
     * @param sequence reference to sequence of macros
     * @param startColor Start color. Use noColor to invoke with default color
     * @param intens Idle intensity for effect. Use nullptr to start with default intensity
     * @param delayedStart Number of steps that are waited before effect is started
     */
    void SetEffect(const Macro &sequence, Color const *startColor, const uint8_t *intens,
                   const uint8_t delayedStart);

    /**
     * @brief Execute step of state-machine
     * @note Tick must be called regularly. For standard light applications all 40 ms seems to be
     *  a convenient values.
     * @return Color color result
     */
    void Tick(Color * color);

    /// @brief Black out lights
    static void UpdateBlank(StepProcessValues_t &pcsParam);
    /// @brief Continuous illumination in idle intensity
    static void UpdateIdle(StepProcessValues_t &pcsParam);
    /// @brief Keep latest state on hold
    static void UpdateFreeze(StepProcessValues_t &pcsParam);
    /// @brief Step through a defined waveform
    static void UpdateWave(StepProcessValues_t &pcsParam);
    /// @brief Step through a defined waveform backwards
    static void UpdateRevWave(StepProcessValues_t &pcsParam);
    /// @brief Show flicker effect with idle intensity as static part with dynamic flicker-range
    static void UpdateFlicker(StepProcessValues_t &pcsParam);

    /// @brief Black out all lights (for multiple pixels)
    static void UpdateBlankAll(StepProcessValues_t &pcsParam); 
    /// @brief Continuous illumination in idle intensity (for multiple pixels)
    static void UpdateIdleAll(StepProcessValues_t &pcsParam);
    /// @brief Step through a defined waveform (for multiple pixels)
    static void UpdateWaveAll(StepProcessValues_t &pcsParam);
    /// @brief Swipe defined waveform  (for multiple pixels)
    static void UpdateRotateWaveAll(StepProcessValues_t &pcsParam);

  protected:
    /**
     * @brief Collection of process values to maintain processing state machine
     */
    struct ProcessValues_t {
        /// @brief Decrementing tick, counting duration
        uint8_t tick;
        /// @brief Decrementing repeats of current Effect-part
        uint8_t repeats;
        /// @brief Dissolve counter dissolves the leading color each tick
        uint8_t dissolveCnt;
    };

    /// @brief First element of Effect macro stack
    const Macro *_p_effSeq;
    /// @brief Currently indexed Effect-part
    const Macro_t *_p_effMac;

    /// @brief Last color
    Color _lastColor;

    /// @brief Current color
    Color _curentColor;

    /// @brief Concentrated parameter values of instance
    MachineParameter_t &_params;

    /// @brief Concentrated process values of instance
    ProcessValues_t _prcValues;

    /// @brief Current index to waveform
    ExactIndex _idx;
};

} // namespace Effect
