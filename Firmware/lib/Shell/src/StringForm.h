#pragma once

#include <string>
#include <vector>
#include <map>
#include <Color.h>
#include "DeviceScenes.h"
#include "DeviceState.h"

namespace StringShell {

extern const std::string cSetEchoCmd;
extern const std::string cGetVersion;
extern const std::string cSetColorCmd;
extern const std::string cSetEffectCmd;
extern const std::string cGetStatusCmd;
extern const std::string cPrompt;

extern const std::map<std::string, const Color &> ColorMap;
extern const std::map<std::string, VisualizationSate> InputToStateMap;
extern const std::map<VisualizationSate, std::string> StateToEffectMap;
extern const std::map<Button::State, std::string> ButtonStateToStringMap;

}
