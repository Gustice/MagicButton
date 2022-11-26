#pragma once

#include <stdint.h>
/*
    Protocol-definition for USB-CDC-Interface
    Command:
    [CmdId][Flags][Length][Payload Byte 0][Payload Byte ..]
    Examples:
    GetVersion: 0x 01 00 00
    SetColor:   0x 02 00 04 rr gg bb ww
    SerEffect:  0x 03 00 01 ee
    GetStatus:  0x 04 00 00
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
    uint8_t doneBit : 1;
    uint8_t spare : 5;
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

extern const CmdFlags_t EmptyMsgFlag;
extern const CmdFlags_t DoneMsgFlag;
extern const CmdFlags_t ErrorEventMsgFlag;
extern const CmdFlags_t EventMsgFlag;
extern const CmdFlags_t ErrorMsgFlag;

} // namespace RawShell