using System.Collections.Generic;

namespace ComBridge
{
    public class LogMessage
    {
        public LogTopic Type { get; }
        public string TypeCode { get; }
        public string Message { get; }

        public static readonly Dictionary<LogTopic, string> TypeCodes = new Dictionary<LogTopic, string>()
        {
            { LogTopic.Message, "🔹⬅️  "},
            { LogTopic.Request , "💻➡️  "},
            { LogTopic.Response , "  ⬅️🔘"},
            { LogTopic.Error , "⚠️⬅️  "},
            { LogTopic.Event , "⚡⬅️  "},
        };

        public LogMessage(LogTopic type, string message)
        {
            Type = type;
            TypeCode = TypeCodes[type];
            Message = message;
        }
    }
}
