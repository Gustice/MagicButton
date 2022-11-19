using ComBridge;
using EvalApplication.Model;
using Prism.Commands;
using Prism.Mvvm;
using System;
using System.Collections.Generic;
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
            Items.Add(new LogMessage(LogType.Message, "Some Message"));
            Items.Add(new LogMessage(LogType.Request, "Some Request"));
            Items.Add(new LogMessage(LogType.Response, "Some Response"));
            Items.Add(new LogMessage(LogType.Error, "Some Error"));
            Items.Add(new LogMessage(LogType.Request, "Some Event"));
        }

        public LogControlViewModel(Action<ComButton.TransferMode> newMode)
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
            var item = new LogMessage(/*CharToTypeCode[message[0]]*/LogType.Event, message);
            _dispatcher.Invoke(() => Items.Add(item));
        }
    }
}
