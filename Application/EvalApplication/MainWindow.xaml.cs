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

        public MainWindow()
        {
            InitializeComponent();

            LogControl = new LogControlViewModel(ComButton.TransferMode.Ascii);
            DeviceControl = new DeviceControlViewModel(LogControl);

            DataContext = this;
        }
    }
}
