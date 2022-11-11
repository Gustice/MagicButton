#pragma once

#include <stdint.h>
/*
    Protocol-definition for USB-CDC-Interface
    Command:
    [CmdId][Flags][Length][Payload Byte 0][Payload Byte ..]
*/

namespace RawShell {

enum RawCommands { 
    UndefinedCommand = 0, 
    GetVersion = 1, 
    SetColor, 
    SetEffect, 
    GetStatus,
    RawCommandCount
};

enum RawEvents { // Events start at 0x80
    ButtonEvent = 0x81,
    ErrorEvent,
};

struct CmdHeader_t {
    uint8_t command; // Command e_Commands
    uint8_t flags;
    uint8_t length; // Message length
};

struct CmdFlags_t {
    uint8_t spare : 6;
    uint8_t eventBit : 1;
    uint8_t errorBit : 1;
};

struct SetColorCommand_t {
    CmdHeader_t Cmd;
    uint8_t Rgbw[4];
};

struct SetEffect_t {
    CmdHeader_t Cmd;
    uint8_t Effect;
};

} // namespace RawShell