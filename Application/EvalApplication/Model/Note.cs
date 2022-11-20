using System.Collections.Generic;

namespace EvalApplication.Model
{
    public class Note
    {
        static Dictionary<NoteType, string> TypeSignes = new Dictionary<NoteType, string>()
        {
            {NoteType.Info, "ℹ️" },
            {NoteType.Warning, "⚠️" },
            {NoteType.Error, "‼️" },
        };

        public enum NoteType
        {
            Info,
            Warning,
            Error,
        }

        public NoteType Type { get; }
        public string Sign { get; }
        public string Message { get; }

        public Note()
        {
            Type = NoteType.Info;
            Sign = "";
            Message = "";
        }

        public Note(NoteType type, string message)
        {
            Type = type;
            Sign = TypeSignes[type];
            Message = message;
        }
    }
}
