#pragma once

#include "DeviceState.h"
#include "Processor.h"
#include <map>


namespace Effect {

struct Scene {
    const Macro &Effect;
    const Color &color;
};

extern const std::map<VisualizationSate, Scene> Scenes;

} // namespace Effect


