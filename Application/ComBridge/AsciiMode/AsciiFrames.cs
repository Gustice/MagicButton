using System.Collections.Generic;

namespace ComBridge.AsciiMode
{
    internal class AsciiFrames
    {
        public const string GetVersion = "Get Version:";
        public const string SetColorCmd = "Set Color:";
        public const string SetEffectCmd = "Set Effect:";
        public const string GetStatusCmd = "Get Status:";

        readonly Dictionary<string, ComButton.VisualizationSate> InputToStateMap = new Dictionary<string, ComButton.VisualizationSate> {
            { "Idle",ComButton.VisualizationSate.Idle}, 
            { "Processing",ComButton.VisualizationSate.Processing},
            { "Good",ComButton.VisualizationSate.Good}, 
            { "Bad",ComButton.VisualizationSate.Bad},
        };

        readonly Dictionary<ComButton.VisualizationSate, string> StateToEffectMap = new Dictionary<ComButton.VisualizationSate, string> {
            {ComButton.VisualizationSate.Startup, "Startup"}, 
            {ComButton.VisualizationSate.Connected, "Connected"},
            {ComButton.VisualizationSate.Idle, "Idle"},
            {ComButton.VisualizationSate.Processing, "Processing"},
            {ComButton.VisualizationSate.Good, "Good"},
            {ComButton.VisualizationSate.Bad, "Bad"},
        };
        
        readonly Dictionary<ComButton.State, string> ButtonStateToStringMap = new Dictionary<ComButton.State, string> {
            { ComButton.State.Idle, "Idle"}, 
            { ComButton.State.Pressed, "Pressed"}, 
            { ComButton.State.Holding, "Holding"}, 
            { ComButton.State.Released, "Released"},
        };
    }
}
