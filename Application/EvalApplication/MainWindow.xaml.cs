using ComBridge;
using EvalApplication.Ux.ViewModels;
using System;
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

        public MainWindow()
        {
            InitializeComponent();

            LogControl = new LogControlViewModel(newMode);
            DeviceControl = new DeviceControlViewModel(LogControl);

            DataContext = this;
        }

        private void newMode(ComButton.TransferMode mode)
        {
            DeviceControl.TransferMode = mode;
        }
    }
}
