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

        public enum Commands
        {
            UndefinedCommand = 0,
            GetVersion = 1,
            SetColor,
            SetEffect,
            GetStatus,
            RawCommandCount
        };

        public enum Events
        {
            ButtonEvent = 0x81,
            ErrorEvent,
        };

        public struct Header_t
        {
            public byte command; // Command e_Commands
            public byte flags;
            public byte length; // Message length
        };


        class SetColorCommand_t
        {
            Header_t Cmd;
            byte[] Rgbw = new byte[4];
        };

        struct SetEffect_t
        {
            Header_t Cmd;
            byte Effect;
        };

        [Flags]
        enum CmdFlags_t : byte
        {
            doneBit = 0x01,
            eventBit = 0x40,
            errorBit = 0x80,
        };
    }
       

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
