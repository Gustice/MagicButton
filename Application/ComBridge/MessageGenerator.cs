using System;
using System.IO.Ports;
using System.Text;
using System.Threading.Tasks;
using static ComBridge.ComButton;

namespace ComBridge
{
    internal abstract class MessageGenerator
    {
        private readonly SerialPort _port;
        protected Action<Dircetion, string> _logTransfer;
        public bool IsActive => _port != null && _port.IsOpen;
        public MessageGenerator(SerialPort port, Action<Dircetion, string> logTransfer)
        {
            _port = port;
            _logTransfer = logTransfer;
        }

        public abstract Task SetVisualizationState(VisualizationSate state);
        public abstract Task SetColor(Color color);
        public abstract Task ReadStates();

        public async Task SendAsciiCommand(string command)
        {
            await WriteAsync(Encoding.ASCII.GetBytes(command + "\n"));
            _logTransfer?.Invoke(Dircetion.ToDevice, ">" + command);
            return;
        }

        protected async Task WriteAsync(byte[] data)
        {
            if (!IsActive)
                return;

            await _port.BaseStream.WriteAsync(data, 0, data.Length);
        }
    }
}
