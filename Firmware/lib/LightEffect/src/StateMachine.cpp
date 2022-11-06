/**
 * @file StateMachine.cpp
 * @author Gustice
 * @brief Implementation of Effect-State-Machine-Class EffectStateMachine.h
 */

#include "StateMachine.h"
#include "EffectWaveforms.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

using namespace Effect;

/// Optional prequel for delayed start.
/// \li Duration must be != 0
/// \li Next entry will be first entry in given Effect sequence
Macro delayPrequel = {{1, 0, StateMachine::UpdateBlank, 0, &CBlack}};

StateMachine::StateMachine(MachineParameter_t &parameter) : _params(parameter), _idx(1, 1) {
    assert(parameter.targetCount > 0);
}

void StateMachine::SetEffect(const Macro &sequence, Color const *startColor, uint8_t initialDelay) {
    SetEffect(sequence, startColor, &_params.idleIntens, initialDelay);
}

void StateMachine::SetEffect(const Macro &sequence, Color const *startColor, const uint8_t *intens,
                             const uint8_t delayedStart) {
    if (startColor != noColor) {
        _curentColor = *startColor;
    } else {
        _curentColor = sequence[0].pColor;
    }
    _lastColor.SetColor(_curentColor.GetColor());
    if (intens != nullptr) {
        _params.idleIntens = *intens;
    }

    _p_effSeq = &sequence;
    if (delayedStart > 0) {
        _p_effMac = &delayPrequel[0];
        _prcValues.tick = delayedStart;
    } else {
        _p_effMac = &sequence[0];
        _prcValues.tick = _p_effMac->Duration;
    }

    _prcValues.tick++;
    _prcValues.dissolveCnt = 0;
    _prcValues.repeats = _p_effMac->Repeats;
    _idx = ExactIndex(_p_effMac->pWave->Length, _p_effMac->Duration);
}

void StateMachine::Tick(Color *color) {
    // tick-increment
    if (--_prcValues.tick == 0) {
        // repeats-decrement
        if (_prcValues.repeats-- == 0) {
            _p_effMac = &(*_p_effSeq)[_p_effMac->Next];
            _prcValues.repeats = _p_effMac->Repeats;

            // execute color change if necessary
            if (_p_effMac->pColor != noColor) {
                _prcValues.dissolveCnt = _params.fadeSteps;
                _lastColor = _curentColor;
                _curentColor = _p_effMac->pColor;
            }
        }

        _prcValues.tick = _p_effMac->Duration;
        _idx = ExactIndex(_p_effMac->pWave->Length, _p_effMac->Duration);
    }

    ++_idx;

    StepProcessValues_t pv{
        .color = color,
        .pCurentColor = _curentColor,
        .ColorLen = _params.targetCount,
        .IdleIntensity = _params.idleIntens,
        .FullIntensity = _p_effMac->FsIntensity,
        .DynamicRange = _params.dynamicRange,
        .waveIdx = _idx.GetIdx(),
        .pWave = _p_effMac->pWave,
    };

    _p_effMac->pImplementation(pv);

    if (_prcValues.dissolveCnt > 0) {
        _prcValues.dissolveCnt--;

        uint8_t dissolving = (uint8_t)((uint16_t)0xFF * _prcValues.dissolveCnt / _params.fadeSteps);
        Color temp = _curentColor;
        temp.MixWith(_lastColor, dissolving);
        *color = temp;
    }
}

void StateMachine::UpdateBlank(StepProcessValues_t &pcsParam) { *pcsParam.color = CBlack; }

void StateMachine::UpdateIdle(StepProcessValues_t &pcsParam) {
    Color temp = pcsParam.pCurentColor * pcsParam.IdleIntensity;
    *pcsParam.color = temp;
}

void StateMachine::UpdateFreeze(StepProcessValues_t &pcsParam) {}

void StateMachine::UpdateWave(StepProcessValues_t &pcsParam) {
    Color temp = pcsParam.pCurentColor;
    temp =
        temp * ((float)(pcsParam.pWave->pWave[pcsParam.waveIdx]) / 0xFF * pcsParam.FullIntensity);
    *pcsParam.color = temp;
}

void StateMachine::UpdateRevWave(StepProcessValues_t &pcsParam) {
    auto lastIndex = pcsParam.pWave->Length - 1;

    Color temp = pcsParam.pCurentColor;
    temp = temp * ((float)(pcsParam.pWave->pWave[lastIndex - pcsParam.waveIdx]) / 0xFF *
                   pcsParam.FullIntensity);
    *pcsParam.color = temp;
}

void StateMachine::UpdateFlicker(StepProcessValues_t &pcsParam) {
    uint8_t k = rand();
    k = pcsParam.IdleIntensity + ((int16_t)k * (pcsParam.DynamicRange) / 0xFF);

    *pcsParam.color = pcsParam.pCurentColor;
    *pcsParam.color = *pcsParam.color * ((float)(k));
}

void StateMachine::UpdateBlankAll(StepProcessValues_t &pcsParam) {
    for (size_t i = 0; i < pcsParam.ColorLen; i++) {
        pcsParam.color[i] = CBlack;
    }
}

void StateMachine::UpdateIdleAll(StepProcessValues_t &pcsParam) {
    Color temp = pcsParam.pCurentColor * pcsParam.IdleIntensity;
    for (size_t i = 0; i < pcsParam.ColorLen; i++) {
        pcsParam.color[i] = temp;
    }
}

void StateMachine::UpdateWaveAll(StepProcessValues_t &pcsParam) {
    Color temp = pcsParam.pCurentColor;
    temp =
        temp * ((float)(pcsParam.pWave->pWave[pcsParam.waveIdx]) / 0xFF * pcsParam.FullIntensity);

    for (size_t i = 0; i < pcsParam.ColorLen; i++) {
        pcsParam.color[i] = temp;
    }
}

void StateMachine::UpdateRotateWaveAll(StepProcessValues_t &pcsParam) {
    Color temp = pcsParam.pCurentColor;
    temp =
        temp * ((float)(pcsParam.pWave->pWave[pcsParam.waveIdx]) / 0xFF * pcsParam.FullIntensity);
    *pcsParam.color = temp;

    uint16_t cStep = pcsParam.waveIdx << 8;
    uint16_t step = (pcsParam.pWave->Length << 8) / pcsParam.ColorLen;
    const uint8_t *pWave = pcsParam.pWave->pWave;
    for (size_t i = 0; i < pcsParam.ColorLen; i++) {
        pcsParam.color[i] = temp;
        Color tempC = pcsParam.pCurentColor;
        pcsParam.color[i] = tempC * pWave[cStep >> 8];
        cStep += step;
    }
}
