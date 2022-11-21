using ComBridge;
using EvalApplication.Ux.Types;
using EvalApplication.Ux.ViewModels;
using Prism.Events;
using System.Windows;

namespace EvalApplication
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public DeviceControlViewModel DeviceControl { get; } 
        public LogControlViewModel LogControl { get; } 
        public StatusBarViewModel StatusBar { get; } 


        public MainWindow(IEventAggregator ea)
        {
            InitializeComponent();

            StatusBar = new StatusBarViewModel(ea);
            LogControl = new LogControlViewModel(StatusBar, newMode);
            DeviceControl = new DeviceControlViewModel(LogControl, StatusBar);

            DataContext = this;

            ea.GetEvent<NoteEvent>().Publish(new Note(Note.NoteType.Info, "Startup successful"));
        }

        private void newMode(ComButton.TransferMode mode)
        {
            DeviceControl.TransferMode = mode;
        }
    }
}
