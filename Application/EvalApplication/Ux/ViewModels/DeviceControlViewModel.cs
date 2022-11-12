using ComBridge;
using EvalApplication.Ux.Types;
using Prism.Commands;
using Prism.Mvvm;
using System.Collections.ObjectModel;
using System.Diagnostics;

namespace EvalApplication.Ux.ViewModels
{
    public class DeviceControlViewModel : BindableBase
    {
        public DelegateCommandBase ConnectCommand { get; }
        public DelegateCommand<string> SetStateCommand { get; }
        public DelegateCommandBase SetColorCommand { get; }
        public DelegateCommandBase ReadStateCommand { get; }
        public DelegateCommandBase SetdropDownCommand { get; }
        
        private ConnectionState _connection;
        private Activation _workSiumlation;
        public ConnectionState Connection
        {
            get => _connection;
            set => SetProperty(ref _connection, value);
        }
        public Activation WorkSiumlation
        {
            get { return _workSiumlation; }
            set { _workSiumlation = value; }
        }


        private ComButton _activeButton;
        public ComButton ActiveButton
        {
            get => _activeButton;
            set => SetProperty(ref _activeButton, value);
        }

        public ObservableCollection<ComButton> AvailableButtons { get; } = new ObservableCollection<ComButton>();

        private ComButton _magicButton;

        public DeviceControlViewModel()
        {

        }

        public DeviceControlViewModel(ComButton magicButton)
        {
            _magicButton = magicButton;

            ConnectCommand = new DelegateCommand(OnConnect);
            SetStateCommand = new DelegateCommand<string>(OnSetState);
            SetColorCommand = new DelegateCommand(OnSetColor);
            ReadStateCommand = new DelegateCommand(OnReadState);
            SetdropDownCommand = new DelegateCommand(OnSetdropDown);
        }

        private void OnSetdropDown()
        {
            Debug.WriteLine("-- OnSetdropDown");
            var buttons = ComButton.GetButtons();
            AvailableButtons.Clear();
            AvailableButtons.AddRange(buttons);
        }

        private void OnConnect()
        {
            Debug.WriteLine("-- OnConnect");
            Connection = ConnectionState.Connected;
        }

        private void OnSetState(string state)
        {
            Debug.WriteLine($"-- OnSetState - {state}");
        }

        private void OnSetColor()
        {
            Debug.WriteLine("-- OnSetColor");
        }


        private void OnReadState()
        {
            Debug.WriteLine("-- OnReadState");
        }

    }
}
