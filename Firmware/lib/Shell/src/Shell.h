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
#include "Color.h"
#include <string>
#include "DeviceState.h"
#include "Button.h"

/*
    Protocol-definition for USB-CDC-Interface
    Command:
    [Id][Flags][Payload]
*/

enum RawCommands {
    SetMode = 0,
    GetInput,
};
struct rawCmdHeader_t {
    uint8_t cmdCode; // Command e_Commands
    uint8_t length;  // Message length
};

struct DevInData_t {
    uint8_t u1_buzz : 1;
    uint8_t u3_spare : 7;
    uint8_t u8_Mode;
};

struct cmdInput_t {
    rawCmdHeader_t Cmd;
    DevInData_t IN;
};

struct cmdMode_t {
    rawCmdHeader_t Cmd;
    uint8_t u8_Mode;
};

struct ValidCommands_t {
    uint8_t u8_cmdCode;
    uint16_t u16_Length;
};

class Shell {
  public:
    typedef void (*SetColorCb)(const Color &c);
    typedef void (*SetSceneCb)(VisualizationSate e);
    Shell(Stream &stream, const DeviceState & device, SetColorCb colorCb, SetSceneCb sceneCb)
        : _stream(stream), _device(device), _fpColorCb(colorCb), _fpSceneCb(sceneCb) {
        _asciCommand.reserve(32);
    }
    void Tick();
    void ConsumeSymbol(int symbol);

    void PrintHelp(void);
    void PrintWelcome(void);
    void SendButtonEvent(Button::State state);

    void PrintEvent(std::string msg);
    void PrintError(std::string msg);

  private:
    Stream &_stream;
    const DeviceState & _device;

    enum InputType_t {
        New = 0,
        Raw,
        Asci,
    };

    InputType_t _inputType = InputType_t::New;
    uint8_t _rawCommand[16];
    unsigned _rawIdx = 0;
    std::string _asciCommand;

    void SetupNewLine();
    void ProcessRaw();
    void ProcessString(std::string &cmd);
    SetColorCb _fpColorCb;
    SetSceneCb _fpSceneCb;
    void PrintResponse(std::string msg);
};
