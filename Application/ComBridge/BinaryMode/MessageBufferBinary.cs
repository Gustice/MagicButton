using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Text;

namespace ComBridge.BinaryMode
{
    internal class MessageBufferBinary : MessageBuffer
    {


        static Dictionary<byte, LogTopic> CharToTypeCode = new Dictionary<byte, LogTopic>()
        {
            //{'*', LogTopic.Message},
        };


        public MessageBufferBinary(SerialPort port, Action<ComButton.ButtonEvent> buttonEvent,
            Action<string> incomingMessage,
            Action<LogMessage> logTransfer)
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
                    _toRead = b + HeaderSize;

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

            _logTransfer?.Invoke(new LogMessage(LogTopic.Message, msgString));

            var command = new RawFrames.Header_t { command = message[0], flags = message[1], length = message[2] };

            if (command.command == (byte)RawFrames.Events.ButtonEvent)
            {
                _buttonEventCb(message[3] == 'R' ? ComButton.ButtonEvent.Pressed : ComButton.ButtonEvent.Released);
                return;
            }
            if (command.command == (byte)RawFrames.Commands.GetStatus)
            {
                //_buttonEventCb(message[3].ToString());
                _incomingMessageCb(msgString);
            }
        }
    }
}
