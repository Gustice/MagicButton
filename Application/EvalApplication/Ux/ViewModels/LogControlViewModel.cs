using ComBridge;
using Prism.Commands;
using Prism.Mvvm;
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
            Items.Add(new LogMessage(LogTopic.Message, "Some Message"));
            Items.Add(new LogMessage(LogTopic.Request, "Some Request"));
            Items.Add(new LogMessage(LogTopic.Response, "Some Response"));
            Items.Add(new LogMessage(LogTopic.Error, "Some Error"));
            Items.Add(new LogMessage(LogTopic.Request, "Some Event"));
        }

        public LogControlViewModel(StatusBarViewModel statusBar)
        {
            ClearCommand = new DelegateCommand(() => Items.Clear());
            _dispatcher = Dispatcher.CurrentDispatcher;
        }

        internal void AddLog(LogMessage message) 
            => _dispatcher.Invoke(() => Items.Add(message));
    }
}
