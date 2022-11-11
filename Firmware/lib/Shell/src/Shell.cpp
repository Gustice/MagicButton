#include "Shell.h"
#include "Version.h"
#include <vector>

using namespace RawShell;
using namespace StringShell;

void Shell::Tick(void) {
    if (_inputType == InputType_t::Raw) {
        _timeOut.Tick();
        if (_rawIdx > 0 && _timeOut.IsTimeOut()) {
            _rawIdx = 0;
            SendRawEvent(RawEvents::ErrorEvent, "TO");
        }
    }
}

void Shell::ConsumeSymbol(int symbol) {
    if (symbol < 0) // eof symbol ...
        return;
    if (_inputType != InputType_t::Raw)
        _stream.print((char)symbol);

    char c = (char)symbol;
    if (c == '\r')
        return; // ignore Windows Return (accept only '\n')

    switch (_inputType) {
    default:
    case InputType_t::New: {
        _rawIdx = 0;
        if (c == EscapeChar) { // ESC-Sign
            _inputType = InputType_t::Raw;
            SendRawResponse(RawCommands::UndefinedCommand, EmptyMsgFlag, (const uint8_t *)"MBTN",
                            4);
            return;
        }

        if (c == '?') {
            PrintHelp();
            SetupNewLine();
            return;
        }

        if (c == '\n') {
            SetupNewLine();
            return;
        }

        if (c < ' ' || c > '~') {
            PrintError("\nunknown character -> discarding line ..");
            SetupNewLine();
            return;
        }

        _asciCommand.push_back(c);
        _inputType = InputType_t::Asci;
    } break;

    case InputType_t::Raw: {
        static CmdHeader_t *rawHeader = (CmdHeader_t *)_rawCommand;
        _timeOut.Reset();
        _rawCommand[_rawIdx++] = c;

        if (_rawIdx >= (rawHeader->length + sizeof(CmdHeader_t))) {
            ProcessRaw(rawHeader, &_rawCommand[sizeof(CmdHeader_t)],
                       (_rawIdx - sizeof(CmdHeader_t)));
            _rawIdx = 0;
        }
    } break;

    case InputType_t::Asci: {
        if (c == '\b') {
            _asciCommand.pop_back();
            return;
        }

        if (c == '\n') {
            ProcessString(_asciCommand);
            SetupNewLine();
            return;
        }

        if (c < ' ' || c > '~') {
            PrintError("\nunknown character -> discarding line ..");
            SetupNewLine();
            return;
        }

        _asciCommand.push_back(c);
    } break;
    }
}

void Shell::SetupNewLine() {
    if (_inputType == InputType_t::Raw)
        return;

    _asciCommand = "";
    _stream.print("MB > ");
    _inputType = InputType_t::New;
}

void Shell::PrintWelcome(void) {
    if (_inputType == InputType_t::Raw) {
        SendRawResponse(RawCommands::UndefinedCommand, EmptyMsgFlag, (const uint8_t *)"MBTN", 4);
        return;
    }

    _stream.println("** Magic Button Shell **");
    _stream.println("* Get Help with typen ?-Symbol");
    SetupNewLine();
}

void Shell::PrintHelp(void) {
    if (_inputType == InputType_t::Raw)
        return;

    _stream.println("** Magic Button Shell HELP **");
    _stream.println("* String-Commands: ");
    _stream.printf("* '%s' -> Prints version\n", cGetVersion.c_str());
    _stream.printf("* '#x' -> Command-Shortcut for tests\n");
    _stream.printf("*      Color-Commands: #r #g #b #c #m #y #w\n");
    _stream.printf("*      Scene-Commands: #0 #1 #2 #3\n");
    _stream.printf("* '%srr,gg,bb,ww' -> Sets Color to given value\n", cSetColorCmd.c_str());
    _stream.printf("*      Value must be hex-formatted and match given pattern\n");
    _stream.printf("* '%s...' -> Sets Visual effect\n", cSetEffectCmd.c_str());
    _stream.printf("*      Scene-Commands: Idle Processing Good Bad\n");
    _stream.printf("*          Idle: Intended to signal idle state \n");
    _stream.printf("*          Processing: Intended for busy-signal \n");
    _stream.printf("*          Good: Intended for successful-signal \n");
    _stream.printf("*          Bad: Intended for failed-signal \n");
    _stream.printf("* '%s' -> Get Status\n", cGetStatusCmd.c_str());
    _stream.printf("*      Status will be printed in form:\n");
    _stream.printf("*      'State:Btn=<state>;Effect=<state>;'\n");
    _stream.println("* ");
    _stream.println("* RAW-Commands: ");
    _stream.println("* The device can be set to raw-command mode.");
    _stream.println("* The first escape-sign that is received turns the shell of");
}

void Shell::ProcessString(const std::string &cmd) {
    if (cmd[0] == '#' && cmd.size() == 2) {
        PrintResponse("Shortcut detected");
        std::string par = cmd.substr(1);
        if (ColorMap.count(par)) {
            _fpColorCb(ColorMap.at(par));
        } else if (InputToStateMap.count(par)) {
            _fpSceneCb(InputToStateMap.at(par));
        } else
            PrintError("Unknown shortcut");
        return;
    } else if (cmd.find(cGetVersion) == 0) {
        PrintResponse(std::string(DeviceString) + " " + VersionString);
    } else if (cmd.find(cSetColorCmd) == 0) {
        std::string par = cmd.substr(cSetColorCmd.size());
        Color c;
        if (!ResolveColorValueString(par, c))
            return;
        _fpColorCb(c);
    } else if (cmd.find(cSetEffectCmd) == 0) {
        std::string par = cmd.substr(cSetEffectCmd.size());
        if (InputToStateMap.count(par)) {
            _fpSceneCb(InputToStateMap.at(par));
        } else
            PrintError("Unknown Effect");
    } else if (cmd == cGetStatusCmd) {
        const std::string &b = ButtonStateToStringMap.at(_device.ButtonState);
        const std::string &e = StateToEffectMap.at(_device.Visualization);
        std::string state = std::string("State:Btn=") + b + ";" + "Effect=" + e + ";";
        PrintResponse(state);
    } else {
        PrintError("Unknown command. See help with '?' ...");
    }
}

void Shell::ProcessRaw(CmdHeader_t *header, uint8_t *payload, unsigned length) {
    if (header->command <= UndefinedCommand || header->command >= RawCommands::RawCommandCount) {
        SendRawResponse(UndefinedCommand, ErrorMsgFlag, (const uint8_t *)"UC", 2);
        return;
    }
    RawCommands cmd = (RawCommands)header->command;

    switch (cmd) {
    case RawCommands::GetVersion: {
        SendRawResponse(cmd, EmptyMsgFlag, (const uint8_t *)VersionString, strlen(VersionString));
    } break;
    case RawCommands::SetColor: {
        if (length < 4) {
            SendRawResponse(cmd, ErrorMsgFlag, (const uint8_t *)"TS", 2);
            return;
        }
        Color color(payload[0], payload[1], payload[2], payload[3]);
        _fpColorCb(color);
        SendRawResponse(cmd, DoneMsgFlag, payload, 4);
    } break;
    case RawCommands::SetEffect: {
        uint8_t eff = payload[0];
        if (eff <= VisualizationSate::Startup || eff >= VisualizationSate::NumberStates) {
            SendRawResponse(cmd, ErrorMsgFlag, (const uint8_t *)"UE", 2);
            return;
        }
        _fpSceneCb((VisualizationSate)eff);
        SendRawResponse(cmd, DoneMsgFlag, (const uint8_t *)&_device.Visualization, 1);
    } break;
    case RawCommands::GetStatus: {
        uint8_t pay[2]{(uint8_t)_device.ButtonState, (uint8_t)_device.Visualization};
        SendRawResponse(cmd, EmptyMsgFlag, pay, sizeof(pay));
    } break;
    default:
        SendRawResponse(UndefinedCommand, ErrorMsgFlag, (const uint8_t *)"EX", 2);
    }
}

void Shell::SendButtonEvent(Button::State state) {
    if (_inputType == InputType_t::Raw) {
        switch (state) {
        case Button::State::Pressed:
            SendRawEvent(RawEvents::ButtonEvent, "R");
            break;

        case Button::State::Released:
            SendRawEvent(RawEvents::ButtonEvent, "F");
            break;

        default:
            break;
        }
    } else {
        switch (state) {
        case Button::State::Pressed:
            PrintEvent("Btn:L->H");
            break;

        case Button::State::Released:
            PrintEvent("Btn:H->L");
            break;

        default:
            break;
        }
    }
}

void Shell::PrintEvent(const std::string &msg) {
    if (_inputType == InputType_t::Raw)
        return;
    _stream.print(":");
    _stream.println(msg.c_str());
}

void Shell::PrintError(const std::string &msg) {
    if (_inputType == InputType_t::Raw)
        return;

    _stream.print("!");
    _stream.println(msg.c_str());
}

void Shell::PrintResponse(const std::string &msg) {
    if (_inputType == InputType_t::Raw)
        return;

    _stream.print("<");
    _stream.println(msg.c_str());
}

bool Shell::ResolveColorValueString(std::string &code, Color &color) {
    int rgbw[4];
    int start = 0;
    for (size_t i = 0; i < 4; i++) {
        std::size_t p = code.find(",", start);
        if (p != std::string::npos) {
            rgbw[i] = std::stoi(code.substr(start, p - start), 0, 16);
            start = p + 1;
        } else {
            if (i == 3) { // last element
                rgbw[i] = std::stoi(code.substr(start), 0, 16);
            } else {
                PrintError("Four comma-separated color-values expected. Input discared!");
                return false;
            }
        }
    }
    color.SetColor(rgbw[0], rgbw[1], rgbw[2], rgbw[3]);
    return true;
}

void Shell::SendRawEvent(RawEvents type, const std::string &code) {
    _stream.write((uint8_t)type);
    uint8_t flags = 0;

    switch (type) {
    case RawEvents::ButtonEvent:
        flags = *((const uint8_t *)&EventMsgFlag);
        break;
    case RawEvents::ErrorEvent:
        flags = *((const uint8_t *)&ErrorEventMsgFlag);
        break;
    }
    _stream.write(flags);
    _stream.write(code.size());
    _stream.print(code.c_str());
}

void Shell::SendRawResponse(RawCommands type, const CmdFlags_t &flags, const uint8_t *buffer,
                            unsigned length) {
    _stream.write((uint8_t)type);
    _stream.write(*((const uint8_t *)&flags));
    _stream.write(length);
    _stream.write(buffer, length);
}
