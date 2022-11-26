using ComBridge;
using System;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Threading.Tasks;

namespace EvalApplication.Terminal
{
    internal class Program
    {

        static void Main(string[] args)
        {
            ComButton button = QueryUserForComPort();

            SerialPort port = button.ConnectDirectly();
            var userIn = Console.In;
            var userOut = Console.Out;
            
            Task.Run(() => SendProcess(port, userIn));
            Task.Run(() => ReceiveProcess(port, userOut));

            while (true)
            {

            }
        }

        static void SendProcess(SerialPort port, TextReader userIn)
        {
            while (true)
            {
                var c = userIn.Read();
                if (c != -1)
                    port.BaseStream.WriteByte((byte)c);
            }
        }

        static void ReceiveProcess(SerialPort port, TextWriter userOut)
        {
            while (true)
            {
                var c = port.BaseStream.ReadByte();
                if (c != -1)
                    userOut.Write((char)c);
            }
        }

        private static ComButton QueryUserForComPort()
        {
            ComButton button = null;
            do
            {
                var buttons = ComButton.GetButtons();
                Console.WriteLine("Write COMx to connect to COM-Port 'x'");
                Console.WriteLine("Available buttons are");
                foreach (var b in buttons)
                {
                    Console.WriteLine($"    {b.Com} ({b.Device})");
                }
                Console.WriteLine("Type 'R' to refresh connected buttons");
                Console.WriteLine("Type 'E' to exit");
                var input = Console.ReadLine().Trim();

                if (input == "R")
                    continue;

                if (input == "E")
                    Environment.Exit(0);

                input = input.ToLower();
                button = buttons.FirstOrDefault(x => x.Com.ToLower() == input);
                if (button != null)
                    break;

                Console.WriteLine("Please input correct com-Port");
            } while (button == null);
            return button;
        }
    }
}
