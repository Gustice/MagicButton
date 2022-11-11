#include "DeviceScenes.h"

namespace Effect {

const std::map<VisualizationSate, Scene> Scenes {
    {VisualizationSate::Startup, Scene{macStartIdleAll, CWhite}},
    {VisualizationSate::Connected, Scene{macIdleAll, CCyan}},
    {VisualizationSate::Idle, Scene{macIdleAll, CWhite}},
    {VisualizationSate::Processing, Scene{macStdRotate, CYellow}},
    {VisualizationSate::Good, Scene{macStdPulseAll, CGreen}},
    {VisualizationSate::Bad, Scene{macNervousPulseAll, CRed}},
};
}