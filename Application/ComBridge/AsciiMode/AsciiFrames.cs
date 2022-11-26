using System.Collections.Generic;
using System.Linq;

namespace ComBridge.AsciiMode
{
    internal class AsciiFrames
    {
        public const string SetEcho = "Set Echo:";
        public const string GetVersion = "Get Version:";
        public const string SetColorCmd = "Set Color:";
        public const string SetEffectCmd = "Set Effect:";
        public const string GetStatusCmd = "Get Status:";
        public const string PromptInit = "MB ";

        public readonly static  Dictionary<ComButton.VisualizationSate, string> StateToEffectMap = new Dictionary<ComButton.VisualizationSate, string> {
            {ComButton.VisualizationSate.Startup, "Startup"}, 
            {ComButton.VisualizationSate.Connected, "Connected"},
            {ComButton.VisualizationSate.Idle, "Idle"},
            {ComButton.VisualizationSate.Busy, "Processing"},
            {ComButton.VisualizationSate.Good, "Good"},
            {ComButton.VisualizationSate.Fail, "Bad"},
        };

        public readonly static Dictionary<ComButton.State, string> ButtonStateToStringMap = new Dictionary<ComButton.State, string> {
            { ComButton.State.Idle, "Idle"}, 
            { ComButton.State.Pressed, "Pressed"}, 
            { ComButton.State.Holding, "Holding"}, 
            { ComButton.State.Released, "Released"},
        };

        public readonly static Dictionary<string, ComButton.VisualizationSate> EffectStringToStateMap = StateToEffectMap.ToDictionary(p => p.Value, p => p.Key);
        public readonly static Dictionary<string, ComButton.State> ButtonStringToStateMap = ButtonStateToStringMap.ToDictionary( p=> p.Value, p => p.Key);
    }
}
