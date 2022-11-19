using System.Collections.Generic;

namespace EvalApplication.Model
{
    public class LogMessage
    {
        public LogType Type { get; }
        public string TypeCode { get; }
        public string Message { get; }

        public static readonly Dictionary<LogType, string> TypeCodes = new Dictionary<LogType, string>()
        {
            { LogType.Message, "🔹⬅️  "},
            { LogType.Request , "💻➡️  "},
            { LogType.Response , "  ⬅️🔘"},
            { LogType.Error , "⚠️⬅️  "},
            { LogType.Event , "⚡⬅️  "},
        };

        public LogMessage(LogType type, string message)
        {
            Type = type;
            TypeCode = TypeCodes[type];
            Message = message;
        }
    }
}
