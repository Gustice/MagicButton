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

        internal enum State
        {
            Idle,
            Pressed,
            Holding,
            Released,
        }

        public class Color
        {
            byte Red;
            byte Green;
            byte Blue;
            byte White;
        }

        public string Com { get; set; }
        public string Device { get; set; }
        public Action<string> _incommingMessageCb { get; private set; }
        public Action<string> _buttonEventCb { get; private set; }

        SerialPort _port = null;
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
            _incommingMessageCb = incomingMessage;
        }

        public async Task SetVisualizationState(VisualizationSate state)
        {
            var cmd = $"{AsciiFrames.SetEffectCmd}{AsciiFrames.StateToEffectMap[state]}\n";
            await WriteAsync(Encoding.ASCII.GetBytes(cmd));

            return;
        }

        void ProcessIncomingMessag(string message)
        {
            if (message.StartsWith(":Btn:"))
            {
                _buttonEventCb(message.Substring(":Btn:".Length) /*new ButtonEvent(ButtonEvent.EventType.Event, stream)*/);
                return;
            }

            _incommingMessageCb(message /*new ButtonEvent(ButtonEvent.EventType.Response, stream)*/);
        }

        async Task WriteAsync(byte[] data)
        {
            if (!IsActive)
                return;

            //DiscardInBuffer();
            await _port.BaseStream.WriteAsync(data, 0, data.Length);
        }


        StringBuilder sBuffer = new StringBuilder();
        private void OnDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            var input = _port.ReadExisting();
            Console.WriteLine(input);

            while (_port.BytesToRead > 0)
            {
                var c = (char)_port.ReadByte();
                if (c == '\r')
                    continue;

                if (c == '\n')
                {
                    ProcessIncomingMessag(sBuffer.ToString());
                    sBuffer = new StringBuilder();
                    continue;
                }

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