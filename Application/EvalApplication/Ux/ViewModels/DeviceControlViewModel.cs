using ComBridge;
using EvalApplication.Ux.Types;
using Prism.Commands;
using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Threading.Tasks;

namespace EvalApplication.Ux.ViewModels
{
    public class DeviceControlViewModel : BindableBase
    {
        public List<double> WorkLoads { get; set; } = new List<double>() { 2, 5, 10, 15 };
        public double WorkLoad { get; set; } = 5;

        public DelegateCommandBase ConnectCommand { get; }
        public DelegateCommand<string> SetStateCommand { get; }
        public DelegateCommandBase SetColorCommand { get; }
        public DelegateCommandBase ReadStateCommand { get; }
        public DelegateCommandBase SetdropDownCommand { get; }
        public DelegateCommandBase SetWorkSimulationCommand { get; }

        private ConnectionState _connection;
        private Activation _workSiumlation;
        public ConnectionState Connection
        {
            get => _connection;
            set => SetProperty(ref _connection, value);
        }
        public Activation WorkSiumlation
        {
            get => _workSiumlation;
            set => SetProperty(ref _workSiumlation, value);
        }

        private ComButton _activeButton;
        public ComButton ActiveButton
        {
            get => _activeButton;
            set => SetProperty(ref _activeButton, value);
        }

        private string _btnState;
        public string BtnState
        {
            get => _btnState;
            set => SetProperty(ref _btnState, value);
        }

        private string _state;
        public string State
        {
            get => _state;
            set => SetProperty(ref _state, value);
        }
        private double _progressPercent = 0.5;
        public double ProgressPercent
        {
            get => _progressPercent;
            set => SetProperty(ref _progressPercent, Math.Min(100, value));
        }


        public ObservableCollection<ComButton> AvailableButtons { get; } = new ObservableCollection<ComButton>();
        public ComButton.Color SetColor { get; set; } = new ComButton.Color();

        public DeviceControlViewModel()
        {

        }

        private ComButton _magicButton;
        public DeviceControlViewModel(ComButton magicButton)
        {
            _magicButton = magicButton;

            ConnectCommand = new DelegateCommand(OnConnect);
            SetStateCommand = new DelegateCommand<string>(OnSetState);
            SetColorCommand = new DelegateCommand(OnSetColor);
            ReadStateCommand = new DelegateCommand(OnReadState);
            SetdropDownCommand = new DelegateCommand(OnSetdropDown);
            SetWorkSimulationCommand = new DelegateCommand(OnSetWorkSimulation);
        }

        private void OnSetdropDown()
        {
            Debug.WriteLine("-- OnSetdropDown");
            var buttons = ComButton.GetButtons();
            AvailableButtons.Clear();
            AvailableButtons.AddRange(buttons);
        }

        private async void OnConnect()
        {
            Debug.WriteLine("-- OnConnect");
            try
            {
                await ActiveButton.Connect(UpdateButtonState, UpdateStatusMessage);
                Connection = ConnectionState.Connected;
            }
            catch (System.Exception)
            {
                Connection = ConnectionState.FailedToConnect;
            }
        }

        private async void OnSetState(string state)
        {
            Debug.WriteLine($"-- OnSetState - {state}");
            try
            {
                var s = (ComButton.VisualizationSate)Enum.Parse(typeof(ComButton.VisualizationSate), state);
                await ActiveButton.SetVisualizationState(s);
            }
            catch (Exception)
            {
                Debug.WriteLine($"! Failed to set state");
            }
        }

        private async void OnSetColor()
        {
            Debug.WriteLine("-- OnSetColor");
            await ActiveButton.SetColor(SetColor);
        }

        private async void OnReadState()
        {
            Debug.WriteLine("-- OnReadState");
            await ActiveButton.ReadStates();
        }

        private async void UpdateButtonState(string state)
        {
            try
            {
                BtnState = state;
                if (WorkSiumlation != Activation.Active)
                    return;
                if (state != "L->H")
                    return;

                await ActiveButton.SetVisualizationState(ComButton.VisualizationSate.Busy);
                await SimulateTask(WorkLoad * 1000);
                await ActiveButton.SetVisualizationState(ComButton.VisualizationSate.Good);
                await Task.Delay(5000);
                await ActiveButton.SetVisualizationState(ComButton.VisualizationSate.Idle);
            }
            catch (Exception)
            {

            }

        }
        private void UpdateStatusMessage(string state)
        {
            State = state;
        }
        private void OnSetWorkSimulation()
        {
            if (WorkSiumlation == Activation.Passiv)
                WorkSiumlation = Activation.Active;
            else
                WorkSiumlation = Activation.Passiv;
        }

        private async Task SimulateTask(double time)
        {
            double progress = 0;
            while (progress < time)
            {
                await Task.Delay(100);
                progress += 100;
                ProgressPercent = 100.0 * (progress / time);
            }
            ProgressPercent = 0;
            return;
        }
    }
}
