using System;
using System.IO.Ports;
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
            await WriteAsync(new byte[] { 0x04, 0x00, 0x00 });
        }

        public override async Task SetColor(Color color)
        {
            await WriteAsync(new byte[] { 0x02, 0x00, 0x04, color.Red, color.Green, color.Blue, color.White });
        }

        public override async Task SetVisualizationState(VisualizationSate state)
        {
            await WriteAsync(new byte[] { 0x03, 0x00, 0x01, (byte)state });
        }
    }
}
