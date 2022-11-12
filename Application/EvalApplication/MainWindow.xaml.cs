using ComBridge;
using EvalApplication.Ux.ViewModels;
using System.Windows;
using System.Windows.Controls;

namespace EvalApplication
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public ComButton MagicButton { get; } = new ComButton();
        public DeviceControlViewModel DeviceControl { get; } 
        public LogControlViewModel LogControl { get; } 

        public MainWindow()
        {
            InitializeComponent();

            DeviceControl = new DeviceControlViewModel(MagicButton);
            LogControl = new LogControlViewModel();

            DataContext = this;
        }
    }
}
