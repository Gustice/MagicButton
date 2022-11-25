using Prism.Commands;
using Prism.Mvvm;
using Application.Defines;
using System.Diagnostics;
using ComBridge;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using System;
using Application.Models;

namespace Application.ViewModels
{
    public class MainViewModel : BindableBase
    {
        private ConnectionState _connection;
        public ConnectionState Connection
        {
            get => _connection;
            set => SetProperty(ref _connection, value);
        }

        public DelegateCommandBase ConnectCommand { get; }
        bool CanConnect() => ActiveButton != null && Connection == ConnectionState.Initial;
        public DelegateCommandBase SetdropDownCommand { get; }
        public DelegateCommandBase AddNewActionCommand { get; }
        public DelegateCommand<ButtonAction> DeleteItemCommand { get; }

        private ComButton _activeButton;
        public ComButton ActiveButton
        {
            get => _activeButton;
            set => SetProperty(ref _activeButton, value);
        }

        private string _btnState = "▫️";
        public string BtnState
        {
            get => _btnState;
            set => SetProperty(ref _btnState, value);
        }

        private string _executionPath;
        public string ExecutionPath
        {
            get => _executionPath;
            set => SetProperty(ref _executionPath, value);
        }

        private ComButton.ButtonEvent _btnEvent;
        public ComButton.ButtonEvent BtnEvent
        {
            get => _btnEvent;
            set => SetProperty(ref _btnEvent, value);
        }

        private ButtonAction _action;
        public ButtonAction Action
        {
            get => _action;
            set => SetProperty(ref _action, value);
        }

        private ButtonAction _newAction = new ButtonAction();
        public ButtonAction NewAction
        {
            get => _newAction;
            set => SetProperty(ref _newAction, value);
        }

        public ObservableCollection<ComButton> AvailableButtons { get; } = new ObservableCollection<ComButton>();
        public ObservableCollection<ButtonAction> Actions { get; } = new ObservableCollection<ButtonAction>();

        public MainViewModel()
        {
            ConnectCommand = new DelegateCommand(OnConnect, CanConnect).ObservesProperty(() => ActiveButton);
            SetdropDownCommand = new DelegateCommand(OnSetdropDown);
            AddNewActionCommand = new DelegateCommand(OnAddNewAction);
            DeleteItemCommand = new DelegateCommand<ButtonAction>(OnDeleteAction);

            Actions.Add(new ButtonAction() { Name = "Lock Screen", Command = "Rundll32.exe user32.dll,LockWorkStation" });
            Actions.Add(new ButtonAction() { Name = "Open Google", Command = "explorer \"https://google.com\"" });
        }

        private void OnAddNewAction()
        {
            Actions.Add(NewAction);
            NewAction = new ButtonAction();
        }

        private void OnDeleteAction(ButtonAction obj)
        {
            Actions.Remove(obj);
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
                await ActiveButton.Connect(UpdateButtonState, UpdateStatusMessage, ComButton.TransferMode.Binary);
                Connection = ConnectionState.Connected;
            }
            catch (Exception e)
            {
                Connection = ConnectionState.FailedToConnect;
                Debug.WriteLine($"Exception: " + e.Message);
            }
        }

        private void UpdateStatusMessage(string state)
        {

        }

        private async void UpdateButtonState(ComButton.ButtonEvent state)
        {
            try
            {
                BtnState = state == ComButton.ButtonEvent.Pressed ? "🔘" : "⚪";
                BtnEvent = state;

                if (state != ComButton.ButtonEvent.Released)
                    return;

                await ActiveButton.SetVisualizationState(ComButton.VisualizationSate.Busy);
                if (ExecuteCommand(Action.Command))
                    await ActiveButton.SetVisualizationState(ComButton.VisualizationSate.Good);
                else
                    await ActiveButton.SetVisualizationState(ComButton.VisualizationSate.Fail);

                await Task.Delay(5000);
                await ActiveButton.SetVisualizationState(ComButton.VisualizationSate.Idle);
            }
            catch (Exception e)
            {
                Debug.WriteLine($"Exception: " + e.Message);
            }
        }

        bool ExecuteCommand(string command)
        {
            int exitCode;
            ProcessStartInfo processInfo;
            Process process;

            processInfo = new ProcessStartInfo("cmd.exe", "/c " + command);
            processInfo.CreateNoWindow = true;
            processInfo.UseShellExecute = false;
            // Redirect the output
            processInfo.RedirectStandardError = true;
            processInfo.RedirectStandardOutput = true;

            process = Process.Start(processInfo);
            process.WaitForExit();

            // Read the streams
            // Warning: This approach can lead to deadlocks
            string output = process.StandardOutput.ReadToEnd();
            string error = process.StandardError.ReadToEnd();
            exitCode = process.ExitCode;

            Debug.WriteLine("output>>" + (String.IsNullOrEmpty(output) ? "(none)" : output));
            Debug.WriteLine("error>>" + (String.IsNullOrEmpty(error) ? "(none)" : error));
            Debug.WriteLine("ExitCode: " + exitCode.ToString(), "ExecuteCommand");
            process.Close();

            return exitCode == 0;
        }

    }
}
