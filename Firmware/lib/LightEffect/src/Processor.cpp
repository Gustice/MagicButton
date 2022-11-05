/**
 * @file Processor.cpp
 * @author Gustice
 * @brief Implementation of Effect-Processor-Class EffectProcessor.h
 */

#include "Processor.h"
#include "EffectWaveforms.h"
#include "Macro.h"
#include <math.h>
#include <stdlib.h>
#include <string>
#include <algorithm>

using namespace Effect;

Processor::Processor(uint8_t targetCount, uint8_t const fadeSteps)
    : _pColor(), _pColorOld(), _parameter{0x20, 0x20, 0, targetCount}, _effPV1(_parameter),
      _effPV2(_parameter) {

    _EffPV = &_effPV1;
    _EffPV_old = &_effPV2;
    _EffPV->SetEffect(macDark, noColor);
    _fadeSteps = fadeSteps;
}

void Processor::SetEffect(const Macro &sequence, Color const *sColor, uint8_t intens) {
    _fadingCnt = _fadeSteps;

    std::swap(_EffPV, _EffPV_old);
    _EffPV->SetEffect(sequence, sColor, &intens, 0);
}

Color const *Processor::Tick(void) {
    uint8_t k;
    _pColor = _EffPV->Tick();

    if (_fadingCnt > 0) { // always process soft cross dissolve between different macros
        _fadingCnt--;
        k = (uint16_t)_fadingCnt * 0xFF / _fadeSteps;

        _pColorOld = _EffPV_old->Tick();
        crossFadeColors(k);
    }

    return &_pColor;
}

void Processor::crossFadeColors(uint8_t k) {
    Color temp = _pColorOld * k;
    _pColor = _pColor * (0xFF - k);
    _pColor = _pColor + temp;
}