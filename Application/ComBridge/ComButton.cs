using ComBridge.AsciiMode;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Management;
using System.Text;
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
        public Action<string> _incomingMessageCb { get; private set; }
        public Action<string> _buttonEventCb { get; private set; }

        SerialPort _port = null;
        StringBuilder sBuffer = new StringBuilder();
        private Action<Dircetion, string> _logTransfer;

        public bool IsActive => _port != null && _port.IsOpen;

        public async Task Connect(Action<string> buttonEvent, Action<string> incomingMessage)
        {
            _port = new SerialPort();

            _port.PortName = Com;
            _port.BaudRate = 9600; // no effect, because uart interface is not used
            _port.Parity = Parity.None;
            _port.DataBits = 8;
            _port.StopBits = StopBits.One;
            _port.Handshake = Handshake.None;
            _port.Parity = Parity.None;
            //_port.RtsEnable = true;
            _port.DtrEnable = true;

            _port.Open();
            await Task.Delay(1);

            _port.DiscardInBuffer();
            _port.DiscardOutBuffer();
            _port.DataReceived += OnDataReceived;

            _buttonEventCb = buttonEvent;
            _incomingMessageCb = incomingMessage;
        }

        public void AppendLogger(Action<Dircetion, string> logTransfer) => _logTransfer = logTransfer;

        public async Task SetVisualizationState(VisualizationSate state)
        {
            var cmd = $"{AsciiFrames.SetEffectCmd}{AsciiFrames.StateToEffectMap[state]}";
            await SendAsciiCommand(cmd);
            return;
        }

        public async Task SetColor(Color color)
        {
            var cmd = $"{AsciiFrames.SetColorCmd}{color.Red:X2},{color.Green:X2},{color.Blue:X2},{color.White:X2}";
            await SendAsciiCommand(cmd);

            return;
        }

        public async Task ReadStates()
        {
            var cmd = $"{AsciiFrames.GetStatusCmd}";
            await SendAsciiCommand(cmd);

            return;
        }

        void ProcessIncomingMessag(string message)
        {
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
                _incomingMessageCb(message);
            }

            _incomingMessageCb(message /*new ButtonEvent(ButtonEvent.EventType.Response, stream)*/);
        }

        async Task SendAsciiCommand(string command)
        {
            await WriteAsync(Encoding.ASCII.GetBytes(command + "\n"));
            _logTransfer?.Invoke(Dircetion.ToDevice, ">"+command);
            return;
        }

        async Task WriteAsync(byte[] data)
        {
            if (!IsActive)
                return;

            //DiscardInBuffer();
            await _port.BaseStream.WriteAsync(data, 0, data.Length);
        }

        private void OnDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            //var input = _port.ReadExisting();
            //Console.WriteLine(input);

            while (_port.BytesToRead > 0)
            {
                var c = (char)_port.ReadByte();
                if (c == '\r')
                    continue;

                if (c == '\n')
                {
                    var s = sBuffer.ToString();
                    ProcessIncomingMessag(s);
                    _logTransfer?.Invoke(Dircetion.FromDevice, s);
                    sBuffer = new StringBuilder();
                    continue;
                }

                if (sBuffer.Length == AsciiFrames.PromptInit.Length)
                    if (sBuffer.ToString() == AsciiFrames.PromptInit)
                        sBuffer.Clear(); // Clear Shell Prompt
                
                sBuffer.Append(c);
            }
        }

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