#include "DeviceScenes.h"

namespace Effect {

const std::map<DeviceState, Scene> Scenes {
    {DeviceState::Startup, Scene{macStartIdleAll, CWhite}},
    {DeviceState::Connected, Scene{macIdleAll, CCyan}},
    {DeviceState::Idle, Scene{macIdleAll, CWhite}},
    {DeviceState::Processing, Scene{macStdRotate, CYellow}},
    {DeviceState::Good, Scene{macStdPulseAll, CGreen}},
    {DeviceState::Bad, Scene{macNervousPulseAll, CRed}},
};
}