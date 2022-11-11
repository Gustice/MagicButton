#pragma once

#include "Button.h"

enum VisualizationSate {
    Startup = 0,
    Connected,
    Idle,
    Processing,
    Good,
    Bad,
    NumberStates
};

struct DeviceState
{
    VisualizationSate Visualization;
    Button::State ButtonState;
};
