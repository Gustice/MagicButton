using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ComBridge.BinaryMode
{
    internal class RawFrames
    {
        const byte Escape = 0x1B;
        public readonly static byte[] EscapeSequce = { Escape };
    }

    enum RawCommands
    {
        UndefinedCommand = 0,
        GetVersion = 1,
        SetColor,
        SetEffect,
        GetStatus,
        RawCommandCount
    };

    enum RawEvents
    { // Events start at 0x80
        ButtonEvent = 0x81,
        ErrorEvent,
    };

    struct CmdHeader_t
    {
        public byte command; // Command e_Commands
        public byte flags;
        public byte length; // Message length
    };

    struct CmdFlags_t
    {
        //bool doneBit : 1;
        //byte spare : 5;
        //byte eventBit : 1;
        //byte errorBit : 1;
    };

    class SetColorCommand_t
    {
        CmdHeader_t Cmd;
        byte[] Rgbw = new byte[4];
    };

    struct SetEffect_t
    {
        CmdHeader_t Cmd;
        byte Effect;
    };

//    const CmdFlags_t EmptyMsgFlag {
//    .doneBit = 0,
//    .spare = 0,
//    .eventBit = 0,
//    .errorBit = 0,
//};

//const CmdFlags_t DoneMsgFlag {
//    .doneBit = 1,
//    .spare = 0,
//    .eventBit = 0,
//    .errorBit = 0,
//};

//const CmdFlags_t ErrorEventMsgFlag {
//    .doneBit = 0,
//    .spare = 0,
//    .eventBit = 1,
//    .errorBit = 1,
//};
//const CmdFlags_t EventMsgFlag {
//    .doneBit = 0,
//    .spare = 0,
//    .eventBit = 1,
//    .errorBit = 0,
//};
//const CmdFlags_t ErrorMsgFlag {
//    .doneBit = 0,
//    .spare = 0,
//    .eventBit = 1,
//    .errorBit = 0,
//};

}
