using ComBridge;
using EvalApplication.Ux.ViewModels;
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


        public MainWindow()
        {
            InitializeComponent();

            StatusBar = new StatusBarViewModel();
            LogControl = new LogControlViewModel(StatusBar, newMode);
            DeviceControl = new DeviceControlViewModel(LogControl, StatusBar);

            DataContext = this;
        }

        private void newMode(ComButton.TransferMode mode)
        {
            DeviceControl.TransferMode = mode;
        }
    }
}
