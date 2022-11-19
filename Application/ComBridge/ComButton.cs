using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Management;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace ComBridge
{
    public class ComButton
    {
        public enum VisualizationSate
        {
            Startup = 0,
            Connected,
            Idle,
            Busy,
            Good,
            Fail,
            NumberStates
        };

        public enum State
        {
            Idle,
            Pressed,
            Holding,
            Released,
        }

        public enum TransferMode
        {
            Ascii,
            Binary,
        }

        public enum Dircetion
        {
            ToDevice,
            FromDevice
        }

        public class Color
        {
            public byte Red { get; set; }
            public byte Green { get; set; }
            public byte Blue { get; set; }
            public byte White { get; set; }
        }

        public string Com { get; set; }
        public string Device { get; set; }

        SerialPort _port = null;
        MessageBuffer _msgBuffer;
        MessageGenerator _msgGenerator;

        private Action<Dircetion, string> _logTransfer;
        private TransferMode _mode;
        protected Action<string> _incomingMessageCb;
        protected Action<string> _buttonEventCb;

        public async Task Connect(Action<string> buttonEvent, Action<string> incomingMessage, ComButton.TransferMode mode)
        {
            _port = new SerialPort();

            _port.PortName = Com;
            _port.BaudRate = 9600; // no effect, because uart interface is not used
            _port.Parity = Parity.None;
            _port.DataBits = 8;
            _port.StopBits = StopBits.One;
            _port.Handshake = Handshake.None;
            _port.Parity = Parity.None;

            _mode = mode;

            _port.Open();
            StartupSequence(_port, _mode);

            await Task.Delay(1);

            _port.DiscardInBuffer();
            _port.DiscardOutBuffer();

            _buttonEventCb = buttonEvent;
            _incomingMessageCb = incomingMessage;
        }

        private void StartupSequence(SerialPort port, TransferMode mode)
        {
            switch (mode)
            {
                case TransferMode.Ascii:
                    _msgBuffer = new AsciiMode.MessageBufferAscii(port, _buttonEventCb, _incomingMessageCb, LogStream);
                    _msgGenerator = new AsciiMode.MessageGeneratorAscii(port, LogStream);
                    break;

                case TransferMode.Binary:
                    _msgBuffer = new BinaryMode.MessageBufferBinary(port, _buttonEventCb, _incomingMessageCb, LogStream);
                    _msgGenerator = new BinaryMode.MessageGeneratorBinary(port, LogStream);
                    port.BaseStream.Write(RawFrames.EscapeSequce, 0, RawFrames.EscapeSequce.Length);
                    // Device is now in binary mode
                    break;

                default:
                    break;
            }

            port.RtsEnable = true;
        }

        public void AppendLogger(Action<Dircetion, string> logTransfer) => _logTransfer = logTransfer;
        private void LogStream(Dircetion dir, string msg) => _logTransfer?.Invoke(dir, msg);

        public async Task SetVisualizationState(VisualizationSate state) 
            => await _msgGenerator?.SetVisualizationState(state);

        public async Task SetColor(Color color) 
            => await _msgGenerator?.SetColor(color);

        public async Task ReadStates()
            => await _msgGenerator?.ReadStates();


        readonly static Regex ComNameMatcher = new Regex(@".*\((?<Com>[^)]*)\)");
        public static List<ComButton> GetButtons()
        {
            var buttons = new List<ComButton>();

            using (var searcher = new ManagementObjectSearcher("SELECT * FROM Win32_PnPEntity WHERE Caption like '%(COM%'"))
            { // Win32_SerialPort class
                var portnames = SerialPort.GetPortNames();
                var ports = searcher.Get().Cast<ManagementBaseObject>().ToList().Select(p => new
                {
                    Caption = p["Caption"].ToString(),
                    ID = p["DeviceID"].ToString(),
                    Description = p["Description"].ToString(),
                    ComPort = ComNameMatcher.Match(p["Caption"].ToString())?.Groups["Com"].Value ?? "",
                });

                ports = ports.Where(p => p.ID.StartsWith(@"USB\VID_0483&PID_5740\"));
                ports = ports.Where(p => portnames.Any(n => n == p.ComPort)); // Com-Port-Name must be valid match

                foreach (var port in ports)
                {
                    buttons.Add(new ComButton() { Com = port.ComPort, Device = port.Description });
                }
            }
            return buttons;
        }

        public override string ToString()
        {
            var name = Device;
            if (name.Length > 16)
                name = name.Substring(0, 12) + " ..";
            return $"{name} ({Com})";
        }
    }
}