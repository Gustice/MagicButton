using ComBridge;
using EvalApplication.Model;
using Prism.Commands;
using Prism.Mvvm;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Windows.Threading;

namespace EvalApplication.Ux.ViewModels
{
    public class LogControlViewModel : BindableBase
    {
        public ObservableCollection<LogMessage> Items { get; } = new ObservableCollection<LogMessage>();
        public DelegateCommandBase ClearCommand { get; }

        private Dispatcher _dispatcher;

        public LogControlViewModel()
        {
            Items.Add(new LogMessage(LogType.Message, "Some Message"));
            Items.Add(new LogMessage(LogType.Request, "Some Request"));
            Items.Add(new LogMessage(LogType.Response, "Some Response"));
            Items.Add(new LogMessage(LogType.Error, "Some Error"));
            Items.Add(new LogMessage(LogType.Request, "Some Event"));
        }

        public LogControlViewModel(ComButton.TransferMode mode)
        {
            ClearCommand = new DelegateCommand(() => Items.Clear());
            _dispatcher = Dispatcher.CurrentDispatcher;
        }


        static Dictionary<char, LogType> CharToTypeCode = new Dictionary<char, LogType>()
        {
            {'*', LogType.Message},
            {'>', LogType.Request},
            {'<', LogType.Response },
            {'!', LogType.Error},
            {':', LogType.Event},
        };

        internal void AddLog(string message)
        {
            var item = new LogMessage(CharToTypeCode[message[0]], message.Substring(1));
            _dispatcher.Invoke(() => Items.Add(item));
        }
    }
}
