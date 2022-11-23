using System;
using System.IO.Ports;

namespace ComBridge
{
    internal abstract class MessageBuffer
    {
        private readonly SerialPort _port;
        protected Action<LogMessage> _logTransfer;
        protected Action<string> _incomingMessageCb;
        protected Action<ComButton.ButtonEvent> _buttonEventCb;

        //var input = _port.ReadExisting();
        //Console.WriteLine(input);

        void ProcessReceiveBuffer(object sender, SerialDataReceivedEventArgs e)
        {
            ProcessReceive(_port);
        }

        public abstract void ProcessReceive(SerialPort port);

        public MessageBuffer(
            SerialPort port,
            Action<ComButton.ButtonEvent> buttonEvent,
            Action<string> incomingMessage,
            Action<LogMessage> logTransfer)
        {
            _port = port;
            _logTransfer = logTransfer;
            _port.DataReceived += ProcessReceiveBuffer;

            _incomingMessageCb = incomingMessage;
            _buttonEventCb = buttonEvent;
        }
    }
}
