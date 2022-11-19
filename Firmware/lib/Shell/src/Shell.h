/**
 * @file Shell.h
 * @author Gustice
 * @brief Shell-Source for interactive control of the device. See docs for further details.
 */

#pragma once

#include "Arduino.h"
#include "Button.h"
#include "Color.h"
#include "DeviceState.h"
#include "RawForm.h"
#include "StringForm.h"
#include "TimeOut.h"
#include <string>

class Shell {
  public:
    static const char EscapeChar = 0x1B;
    typedef void (*SetColorCb)(const Color &c);
    typedef void (*SetSceneCb)(VisualizationSate e);
    Shell(Stream &stream, const DeviceState &device, SetColorCb colorCb, SetSceneCb sceneCb)
        : _stream(stream), _device(device), _fpColorCb(colorCb), _fpSceneCb(sceneCb),
          _timeOut(TimeOutLimit) {
        _asciCommand.reserve(32);
    }
    void Tick(void);
    void ConsumeSymbol(int symbol);

    void PrintHelp(void);
    void PrintWelcome(void);
    void SendButtonEvent(Button::State state);

    void PrintEvent(const std::string &msg);
    void PrintError(const std::string &msg);
    void PrintResponse(const std::string &msg);

  private:
    enum InputType_t {
        New = 0,
        Asci,
        Raw, // Latches in this mode
    };

    unsigned TimeOutLimit = 150;
    Stream &_stream;
    const DeviceState &_device;
    SetColorCb _fpColorCb;
    SetSceneCb _fpSceneCb;

    InputType_t _inputType = InputType_t::New;
    uint8_t _rawCommand[16];
    unsigned _rawIdx = 0;
    std::string _asciCommand;
    TimeOut _timeOut;
    bool _suppressEcho = false;
    bool _disableEcho = false;

    void SetupNewLine();
    void ProcessRaw(RawShell::CmdHeader_t * header, uint8_t * payload, unsigned length);
    void ProcessString(const std::string &cmd);
    bool ResolveColorValueString(std::string &code, Color &color);

    void SendRawEvent(RawShell::RawEvents type, const std::string &code);
    void SendRawResponse(RawShell::RawCommands type, const RawShell::CmdFlags_t & flags, const uint8_t * buffer, unsigned length);
};
