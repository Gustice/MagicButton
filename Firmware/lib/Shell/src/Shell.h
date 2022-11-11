/**
 * @file Shell.h
 * @author Gustice
 * @brief Shell-Source for interactive control of the device
 * @details: # Button-Shell
 *   The device support primarily an interactively usable shell that utilizes
 *   human readable commands.
 *   One can also use raw binary commands in order to avoid the bulky strings.
 *   ## Interactive Shell interface
 *   Each line represents an event, or a command. The commands are separated in
 *   following categories:
 *   - Leading `>`-sign indicates following user input or commands.
 *      Note this sign is only present in the master-terminal, it is not transmitted
 *   - Leading `<`-sign indicates responses to commands after successful processing
 *   - Leading `!`-sign indicates error responses to commands or input
 *   - Leading `:`-sign indicates an acyclic event
 *   <br> In case of user input the `>` is already output by the shell for guidance.
 *   ## Input commands
 *   ## Shortcuts for testing
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

    void SetupNewLine();
    void ProcessRaw(RawShell::CmdHeader_t * header, uint8_t * payload, unsigned length);
    void ProcessString(const std::string &cmd);
    bool ResolveColorValueString(std::string &code, Color &color);

    void SendRawEvent(RawShell::RawEvents type, const std::string &code);
    void SendRawResponse(RawShell::RawCommands type, const RawShell::CmdFlags_t & flags, const uint8_t * buffer, unsigned length);
};
