using System.Collections.Generic;

namespace ComBridge
{
    public class ComButton
    {
        internal enum VisualizationSate
        {
            Startup = 0,
            Connected,
            Idle,
            Processing,
            Good,
            Bad,
            NumberStates
        };

        internal enum State
        {
            Idle,
            Pressed,
            Holding,
            Released,
        }

        public static List<ComButton> GetButtons()
        {
            var buttons = new List<ComButton>();
            for (int i = 0; i < 3; i++)
            {
                buttons.Add(new ComButton() { Name = "Button_" + (i+1) });
            }

            return buttons;
        }

        public string Name { get; set; }

        public override string ToString()
        {
            return Name;
        }
    }
}