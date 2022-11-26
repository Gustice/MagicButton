using System;
using System.IO.Ports;
using System.Text;
using System.Threading.Tasks;
using static ComBridge.ComButton;

namespace ComBridge.BinaryMode
{
    internal class MessageGeneratorBinary : MessageGenerator
    {
        public MessageGeneratorBinary(SerialPort port, Action<LogMessage> logTransfer) : base(port, logTransfer)
        {
            port.BaseStream.Write(RawFrames.EscapeSequce, 0, RawFrames.EscapeSequce.Length);
            // Device is now in binary mode
        }

        public override async Task ReadStates()
        {
            var msg = new byte[] { 0x04, 0x00, 0x00 };
            _logTransfer?.Invoke(new LogMessage(LogTopic.Request, CreateLogStream(msg)));

            await WriteAsync(msg);
        }

        public override async Task SetColor(Color color)
        {
            var msg = new byte[] { 0x02, 0x00, 0x04, color.Red, color.Green, color.Blue, color.White };
            _logTransfer?.Invoke(new LogMessage(LogTopic.Request, CreateLogStream(msg)));

            await WriteAsync(msg);
        }

        public override async Task SetVisualizationState(VisualizationSate state)
        {
            var msg = new byte[] { 0x03, 0x00, 0x01, (byte)state };
            _logTransfer?.Invoke(new LogMessage(LogTopic.Request, CreateLogStream(msg)));

            await WriteAsync(msg);
        }

        private string CreateLogStream(byte[] message)
        {
            var sb = new StringBuilder(message.Length * 4 + 4);
            sb.Append("x0 ");
            foreach (byte b in message)
                sb.Append($"{b:X2}, ");
            var msgString = sb.ToString().TrimEnd(new char[] { ' ', ',' });

            return msgString;
        }
    }
}
