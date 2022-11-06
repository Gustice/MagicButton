/**
 * @file EffectMacro.cpp
 * @author Gustice
 * @brief Predefined Macros
 */

#include "Macro.h"
#include "EffectWaveforms.h"
#include "StateMachine.h"

namespace Effect {

const WaveForm_t EmptyWave{{0}, 1};
const WaveForm_t OffsetPulseWave{gau8_offsetPulse, cu16_TemplateLength};
const WaveForm_t InitSlopeWave{gau8_initSlope, cu16_TemplateLength};
const WaveForm_t OffsetSlopeWave{gau8_offsetSlope, cu16_TemplateLength};
const WaveForm_t FullSlopeWave{gau8_fullSlope, cu16_TemplateLength};
const WaveForm_t FullPulseWave{gau8_fullPulse, cu16_TemplateLength};

/**
 * Macros for single pixels
 */

const Macro macDark = {
    {32, 0, StateMachine::UpdateBlank, 0, &CBlack},
};

const Macro macIdle = {
    {32, 1, StateMachine::UpdateIdle, 0xFF, noColor},
    {32, 1, StateMachine::UpdateIdle, 0xFF, oldColor},
};

const Macro macStartIdle = {
    {24, 1, &InitSlopeWave, StateMachine::UpdateWave, 0xFF, &CColdWhite},
    {32, 1, StateMachine::UpdateIdle, 0xFF, oldColor},
};

const Macro macStartFull = {
    {24, 1, &FullSlopeWave, StateMachine::UpdateWave, 0xFF, &CColdWhite},
    {32, 1, StateMachine::UpdateIdle, 0xFF, oldColor},
};

const Macro macStdPulse = {
    {32, 1, StateMachine::UpdateIdle, 0, noColor},
    {32, 2, &OffsetPulseWave, StateMachine::UpdateWave, 0xFF, oldColor},
    {32, 0, StateMachine::UpdateIdle, 0, oldColor},
};

const Macro macNervousPulse = {
    {8, 1, StateMachine::UpdateIdle, 0, noColor},
    {16, 2, &OffsetPulseWave, StateMachine::UpdateWave, 0xFF, oldColor},
    {8, 0, StateMachine::UpdateIdle, 0, oldColor},
};

const Macro macStdWipe = {
    {32, 1, StateMachine::UpdateIdle, 0, &CColdWhite},

    {32, 2, &OffsetSlopeWave, StateMachine::UpdateWave, 0xFF, oldColor},
    {32, 3, StateMachine::UpdateFreeze, 0, oldColor},
    {32, 4, &OffsetSlopeWave, StateMachine::UpdateRevWave, 0xFF, oldColor},
    {32, 1, StateMachine::UpdateFreeze, 0, oldColor},
};

/**
 * Macros for pixel groups
 */

const Macro macDarkAll = {
    {32, 0, StateMachine::UpdateBlankAll, 0, &CBlack},
};

const Macro macIdleAll = {
    {32, 1, StateMachine::UpdateIdleAll, 0xFF, noColor},
    {32, 1, StateMachine::UpdateIdleAll, 0xFF, oldColor},
};

const Macro macStartIdleAll = {
    {24, 1, &InitSlopeWave, StateMachine::UpdateWaveAll, 0xFF, &CColdWhite},
    {32, 1, StateMachine::UpdateIdleAll, 0xFF, oldColor},
};

const Macro macStartFullAll = {
    {24, 1, &FullSlopeWave, StateMachine::UpdateWaveAll, 0xFF, &CColdWhite},
    {32, 1, StateMachine::UpdateIdleAll, 0xFF, oldColor},
};

const Macro macStdPulseAll = {
    {32, 1, StateMachine::UpdateIdleAll, 0, noColor},
    {32, 2, &OffsetPulseWave, StateMachine::UpdateWaveAll, 0xFF, oldColor},
    {32, 0, StateMachine::UpdateIdleAll, 0, oldColor},
};

const Macro macStdRotate = {
    {32, 1, StateMachine::UpdateIdleAll, 0, noColor},
    {32, 1, &OffsetPulseWave, StateMachine::UpdateRotateWaveAll, 0xFF, oldColor},
};

const Macro macNervousPulseAll = {
    {8, 1, StateMachine::UpdateIdleAll, 0, noColor},
    {16, 2, &OffsetPulseWave, StateMachine::UpdateWaveAll, 0xFF, oldColor},
    {8, 0, StateMachine::UpdateIdleAll, 0, oldColor},
};

/**
 * Macros for processor groups
 */

const std::vector<Macro> macDualStdWip = {
    {
        {32, 1, StateMachine::UpdateIdle, 0, &CColdWhite},

        {32, 2, &OffsetSlopeWave, StateMachine::UpdateWave, 0xFF},
        {32, 3, StateMachine::UpdateFreeze, 0},
        {32, 4, &OffsetSlopeWave, StateMachine::UpdateRevWave, 0xFF},
        {32, 1, StateMachine::UpdateFreeze, 0},
    },
    {
        {48, 1, StateMachine::UpdateIdle, 0, &CColdWhite}, // different initial time

        {32, 2, &OffsetSlopeWave, StateMachine::UpdateWave, 0xFF},
        {32, 3, StateMachine::UpdateFreeze, 0},
        {32, 4, &OffsetSlopeWave, StateMachine::UpdateRevWave, 0xFF},
        {32, 1, StateMachine::UpdateFreeze, 0},
    },

};
}