#include "StringForm.h"

namespace StringShell {

const std::string cSetEchoCmd("Set Echo:");
const std::string cGetVersion("Get Version:");
const std::string cSetColorCmd("Set Color:");
const std::string cSetEffectCmd("Set Effect:");
const std::string cGetStatusCmd("Get Status:");
const std::string cPrompt("$ >");

const std::map<std::string, const Color &> ColorMap{
    {"r", CRed},  {"g", CGreen},  {"b", CBlue},  {"m", CMagenta},
    {"c", CCyan}, {"y", CYellow}, {"w", CWhite},
};
const std::map<std::string, VisualizationSate> InputToStateMap{
    {"0", VisualizationSate::Idle},    {"1", VisualizationSate::Processing},
    {"2", VisualizationSate::Good},    {"3", VisualizationSate::Bad},
    {"Idle", VisualizationSate::Idle}, {"Processing", VisualizationSate::Processing},
    {"Good", VisualizationSate::Good}, {"Bad", VisualizationSate::Bad},
};

const std::map<VisualizationSate, std::string> StateToEffectMap{
    {VisualizationSate::Startup, "Startup"}, {VisualizationSate::Connected, "Connected"},
    {VisualizationSate::Idle, "Idle"},       {VisualizationSate::Processing, "Processing"},
    {VisualizationSate::Good, "Good"},       {VisualizationSate::Bad, "Bad"},
};
const std::map<Button::State, std::string> ButtonStateToStringMap{
    {Button::State::Idle, "Idle"},
    {Button::State::Pressed, "Pressed"},
    {Button::State::Holding, "Holding"},
    {Button::State::Released, "Released"},
};
}