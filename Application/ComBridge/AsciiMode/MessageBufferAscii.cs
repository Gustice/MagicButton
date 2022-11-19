using System;
using System.IO.Ports;
using System.Text;
using static ComBridge.ComButton;

namespace ComBridge.AsciiMode
{
    internal class MessageBufferAscii : MessageBuffer
    {
        public MessageBufferAscii(SerialPort port, Action<string> buttonEvent,
            Action<string> incomingMessage,
            Action<Dircetion, string> logTransfer)
            : base(port, buttonEvent, incomingMessage, logTransfer)
        {
        }

        StringBuilder _buffer = new StringBuilder();

        public override void ProcessReceive(SerialPort port)
        {
            while (port.BytesToRead > 0)
            {
                var c = (char)port.ReadByte();
                if (c == '\r')
                    continue;

                if (c == '\n')
                {
                    var s = _buffer.ToString();
                    ProcessIncomingMessag(s);
                    _buffer = new StringBuilder();
                    continue;
                }

                if (_buffer.Length == AsciiFrames.PromptInit.Length)
                    if (_buffer.ToString() == AsciiFrames.PromptInit)
                        _buffer.Clear(); // Clear Shell Prompt

                _buffer.Append(c);
            }
        }

        void ProcessIncomingMessag(string message)
        {
            _logTransfer?.Invoke(Dircetion.FromDevice, message);

            if (message.StartsWith(":Btn:"))
            {
                _buttonEventCb(message.Substring(":Btn:".Length) /*new ButtonEvent(ButtonEvent.EventType.Event, stream)*/);
                return;
            }
            if (message.StartsWith("<State:"))
            {
                var sStr = message.Substring("<State:".Length);
                var fields = sStr.Split(';');
                _buttonEventCb(fields[0]);
            }

            _incomingMessageCb(message /*new ButtonEvent(ButtonEvent.EventType.Response, stream)*/);
        }
    }
}
