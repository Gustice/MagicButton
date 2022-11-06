#pragma once

#include "Arduino.h"
#include "Color.h"
#include <string>
/*
    Protocol-definition for USB-CDC-Interface
    Command:
    [Id][Flags][Payload]
*/

enum eEffects {
    Eff_Done = -1,
    Eff_Dark = 0,
    Eff_Idle,
    Eff_Process,
    Eff_Fail,
    Eff_Unstable,
    Eff_Stable,
    Eff_NumEffects
};

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
    typedef void (*SetSceneCb)(unsigned e);
    Shell(Stream &stream, SetColorCb colorCb, SetSceneCb sceneCb)
        : _stream(stream), _fpColorCb(colorCb), _fpSceneCb(sceneCb) {
        _asciCommand.reserve(32);
    }
    void Tick();
    void ConsumeSymbol(int symbol);

    void PrintHelp(void);
    void PrintWelcome(void);

  private:
    Stream &_stream;

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
};
