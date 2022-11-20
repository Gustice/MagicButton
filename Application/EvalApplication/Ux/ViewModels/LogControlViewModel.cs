using ComBridge;
using Prism.Commands;
using Prism.Mvvm;
using System;
using System.Collections.ObjectModel;
using System.Windows.Threading;

namespace EvalApplication.Ux.ViewModels
{
    public class LogControlViewModel : BindableBase
    {
        public ObservableCollection<LogMessage> Items { get; } = new ObservableCollection<LogMessage>();
        public DelegateCommandBase ClearCommand { get; }
        public DelegateCommandBase ToHexModeCommand { get; }

        private Dispatcher _dispatcher;
        private Action<ComButton.TransferMode> _newModeCallback;
        public bool HexModeIsActive => _transferMode == ComButton.TransferMode.Binary;
        
        public LogControlViewModel()
        {
            Items.Add(new LogMessage(LogTopic.Message, "Some Message"));
            Items.Add(new LogMessage(LogTopic.Request, "Some Request"));
            Items.Add(new LogMessage(LogTopic.Response, "Some Response"));
            Items.Add(new LogMessage(LogTopic.Error, "Some Error"));
            Items.Add(new LogMessage(LogTopic.Request, "Some Event"));
        }

        public LogControlViewModel(StatusBarViewModel statusBar, Action<ComButton.TransferMode> newMode)
        {
            ClearCommand = new DelegateCommand(() => Items.Clear());
            ToHexModeCommand = new DelegateCommand(OnToHexMode);
            _dispatcher = Dispatcher.CurrentDispatcher;
            _newModeCallback = newMode;
        }

        ComButton.TransferMode _transferMode = ComButton.TransferMode.Ascii;
        private void OnToHexMode()
        {
            if (_transferMode == ComButton.TransferMode.Binary)
                return;

            _transferMode = ComButton.TransferMode.Binary;
            _newModeCallback(_transferMode);
            RaisePropertyChanged(nameof(HexModeIsActive));
        }

        internal void AddLog(LogMessage message) 
            => _dispatcher.Invoke(() => Items.Add(message));
    }
}
