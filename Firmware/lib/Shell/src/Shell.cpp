#include "Shell.h"
#include <vector>

void Shell::ConsumeSymbol(int symbol) {
    if (symbol < 0) // eof symbol ...
        return;     // todo Tidy-Up here

    char c = (char)symbol;
    if (c == '\r')
        return; // ignore Windows Return (accept only '\n')

    switch (_inputType) {
    default:
    case InputType_t::New:
        _rawIdx = 0;
        if (c == 0x1B) // ESC-Sign
            _inputType = InputType_t::Raw;

        if (c == '?')
            PrintHelp();

        if (c == '\n') {
            SetupNewLine();
            return;
        }

        if (c < ' ' || c > '~') {
            _stream.println("\nunknown character -> discarding line ..");
            SetupNewLine();
            return;
        }

        _asciCommand.push_back(c);
        _inputType = InputType_t::Asci;
        break;

    case InputType_t::Raw:
        _rawCommand[_rawIdx++] = c;
        ProcessRaw();
        break;

    case InputType_t::Asci:
        if (c == '\b') { // Backspace
            _asciCommand.pop_back();
            return;
        }

        if (c == '\n') {
            ProcessString(_asciCommand);
            SetupNewLine();
            return;
        }

        if (c < ' ' || c > '~') {
            _stream.println("\nunknown character -> discarding line ..");
            SetupNewLine();
            return;
        }

        _asciCommand.push_back(c);
        break;
    }
}

void Shell::SetupNewLine() {
    _asciCommand = "";
    _stream.print("MB > ");
    _inputType = InputType_t::New;
}

void Shell::PrintWelcome(void) {
    _stream.println("** Magic Button Shell **");
    _stream.println("* Get Help with typen ?-Symbol");
    SetupNewLine();
}

void Shell::PrintHelp(void) {
    _stream.println("** Magic Button Shell HELP **");
    _stream.println("* Commands: ");
    _stream.println("* ... ");
    _stream.println("* ... ");
    _stream.println("* RAW-Commands: ");
    _stream.println("* ... ");
    _stream.println("* ... ");
    SetupNewLine();
}

void Shell::ProcessRaw() {}

static const std::string cSetColorCmd("Set Color:");
static const std::string cSetEffectCmd("Set Effect:");
static const std::string cSetActionCmd("Set Action:");
static const std::string cGetStatusCmd("Set Action:");

void Shell::ProcessString(std::string &cmd) {
    if (cmd[0] == '#') { // Starts with #
        if (cmd == "#r") {
            _fpColorCb(CRed);
        } else if (cmd == "#g") {
            _fpColorCb(CGreen);
        } else if (cmd == "#b") {
            _fpColorCb(CBlue);
        } else if (cmd == "#m") {
            _fpColorCb(CMagenta);
        } else if (cmd == "#c") {
            _fpColorCb(CCyan);
        } else if (cmd == "#y") {
            _fpColorCb(CYellow);
        } else if (cmd == "#w") {
            _fpColorCb(CWhite);
        } else if (cmd == "#1") {
            _fpSceneCb(1);
        } else if (cmd == "#2") {
            _fpSceneCb(2);
        } else if (cmd == "#3") {
            _fpSceneCb(3);
        }
        return;
    } else if (cmd.find(cSetColorCmd) == 0) { // starts with
        int rgbw[4];
        int start = cSetColorCmd.size();
        for (size_t i = 0; i < 4; i++) {
            auto p = cmd.find(",", start);
            if (p != std::string::npos) {
                rgbw[i] = std::stoi(cmd.substr(start, p - start), 0, 16);
                start = p + 1;
            } else {
                if (i == 3) { // last element
                    rgbw[i] = std::stoi(cmd.substr(start), 0, 16);
                } else {
                    _stream.println("Four comma-separated color-values expected. Input discared!");
                    return;
                }
            }
        }
        _stream.println("Setting Color");
        Color c(rgbw[0], rgbw[1], rgbw[2], rgbw[3]);
        _fpColorCb(c);
    } else if (cmd.find(cSetEffectCmd) == 0) { // starts with
        _stream.println("Setting Effect");
    } else if (cmd.find(cSetActionCmd) == 0) { // starts with
        _stream.println("Setting Action");
    } else if (cmd == cGetStatusCmd) { // starts with
        _stream.println("State: ...");
    } else {
        _stream.println("Unknown command. See help with '?' ...");
    }
}

// ValidCommands_t ServiceCommands[] = {
//     {SET_MODE, sizeof(cmdMode_t)},
//     {GET_INPUTS, sizeof(cmdInput_t)},
// };f
// static const uint8_t CommandsCnt = sizeof(ServiceCommands) / sizeof(ServiceCommands[0]);

// static uint16_t u16_ExtBuffSize = 0u;

// // @pararm[in] u16_BufferSize	external Buffer Size for messages available
// void SER_Setup(uint16_t u16_BufferSize) { u16_ExtBuffSize = u16_BufferSize; }

// DevInData_t DevIn;
// eEffects SetMode = Eff_Dark;
// bool newByte = false;

// #define BUFFER_SIZE 8
// static char au8_msgBuffer[BUFFER_SIZE];
// static int timeOut = 0;
// const int TimeOutThreshold = 100;

// const int HeaderSize = sizeof(cmdHeader_t);

// void serialEvent() { newByte = true; }

// void SER_ProcessCOM(void) {
//     static bool HeaderReceived = false;

//     if (!HeaderReceived) {
//         if (SerialUSB.available() <= 0)
//             timeOut = 0;

//         if (SerialUSB.available() >= HeaderSize) {
//             SerialUSB.readBytes(&au8_msgBuffer[0], HeaderSize);
//             HeaderReceived = true;
//         }
//     } else {
//         cmdHeader_t *vp_Cmd = (cmdHeader_t *)au8_msgBuffer;
//         int length = sizeof(cmdHeader_t) + vp_Cmd->u8_length;

//         if (SerialUSB.available() >= length) {
//             SerialUSB.readBytes(&au8_msgBuffer[HeaderSize], length);
//             HeaderReceived = false;

//             int toSend = ProcessData(&au8_msgBuffer[0], length + HeaderSize);
//             SerialUSB.write(au8_msgBuffer, toSend);
//         }
//     }

//     if (timeOut++ > TimeOutThreshold) {
//         HeaderReceived = false;
//         while (SerialUSB.available() > 0) {
//             SerialUSB.read();
//         }
//         timeOut = 0;
//     }
// }

// void Shell::ProcessRaw() {
//     cmdHeader_t *vp_Cmd = (cmdHeader_t *)_rawCommand;
//     unsigned replyLength = _rawIdx;

//     uint8_t cmd = vp_Cmd->u8_cmd;

//     if ((cmd < CommandsCnt) && (ServiceCommands[cmd].u16_Length <= _rawIdx)) {
//         switch (cmd) {
//         // Set Mode-Command
//         case SET_MODE: {
//             enum eEffects tmp = (eEffects)(((cmdMode_t *)vp_Cmd)->u8_Mode);
//             if ((uint8_t)tmp < (uint8_t)Eff_NumEffects) {
//                 SetMode = tmp;
//             }
//         } break;

//         // GetInputs
//         case GET_INPUTS: {
//             DevInData_t *pdata = &(((cmdInput_t *)vp_Cmd)->IN);
//             memcpy(pdata, &DevIn, sizeof(DevInData_t));
//         } break;

//         default:
//             replyLength = 0u;
//             break;
//         }
//     }
//     return replyLength;
// }

// void SER_SendInput(uint8_t state) {
//     if (!Serial)
//         return;

//     uint8_t buffer[3];
//     buffer[0] = (uint8_t)ASY_INPUT;
//     buffer[1] = 0x01;
//     buffer[2] = state;
//     SerialUSB.write(buffer, sizeof(buffer));
// }