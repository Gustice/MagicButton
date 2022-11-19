using ComBridge.BinaryMode;
using System;
using System.IO.Ports;
using System.Threading.Tasks;
using static ComBridge.ComButton;

namespace ComBridge.AsciiMode
{
    internal class MessageGeneratorAscii : MessageGenerator
    {
        public MessageGeneratorAscii(SerialPort port, Action<Dircetion, string> logTransfer) : base(port, logTransfer)
        {
            
        }

        public override async Task ReadStates()
        {
            var cmd = $"{AsciiFrames.GetStatusCmd}";
            await SendAsciiCommand(cmd);
        }

        public override async Task SetColor(Color color)
        {
            var cmd = $"{AsciiFrames.SetColorCmd}{color.Red:X2},{color.Green:X2},{color.Blue:X2},{color.White:X2}";
            await SendAsciiCommand(cmd);
        }

        public override async Task SetVisualizationState(VisualizationSate state)
        {
            var cmd = $"{AsciiFrames.SetEffectCmd}{AsciiFrames.StateToEffectMap[state]}";
            await SendAsciiCommand(cmd);
        }
    }
}
