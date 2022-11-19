using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Text;
using static ComBridge.ComButton;

namespace ComBridge.BinaryMode
{
    internal class MessageBufferBinary : MessageBuffer
    {
        public MessageBufferBinary(SerialPort port, Action<string> buttonEvent,
            Action<string> incomingMessage,
            Action<Dircetion, string> logTransfer)
            : base(port, buttonEvent, incomingMessage, logTransfer)
        {

        }

        List<byte> _buffer = new List<byte>(16);

        const uint HeaderSize = 3;
        uint _toRead = HeaderSize;

        public override void ProcessReceive(SerialPort port)
        {
            while (port.BytesToRead > 0)
            {
                var b = (byte)port.ReadByte();
                _buffer.Add(b);

                if (_buffer.Count == 3)
                    _toRead = b;

                if (_buffer.Count == _toRead)
                {
                    ProcessIncomingBytes(_buffer.ToArray());
                    _buffer = new List<byte>(16);
                    _toRead = HeaderSize;
                }
            }
        }

        void ProcessIncomingBytes(byte[] message)
        {
            var sb = new StringBuilder(message.Length * 4 + 4);
            sb.Append("x0 ");
            foreach (byte b in message)
                sb.Append($"{b:X2}, ");
            var msgString = sb.ToString().TrimEnd(new char[] { ' ', ',' });
            _logTransfer?.Invoke(Dircetion.FromDevice, msgString);

            var command = new CmdHeader_t { command = message[0], flags = message[1], length = message[2] };

            if (command.command == (byte)RawEvents.ButtonEvent)
            {
                _buttonEventCb(message[3].ToString());
                return;
            }
            if (command.command == (byte)RawCommands.GetStatus)
            {
                _buttonEventCb(message[3].ToString());
            }

            _incomingMessageCb(msgString);
        }
    }
}
