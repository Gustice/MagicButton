using Prism.Commands;
using Prism.Mvvm;

namespace Application.ViewModels
{
    public class MainViewModel : BindableBase
    {
        private string _text;
        public string Text
        {
            get => _text;
            set => SetProperty(ref _text, value);
        }

        public DelegateCommandBase ChangeTextCommand { get; }

        public MainViewModel()
        {
            ChangeTextCommand = new DelegateCommand(OnChangeTextCommand);
        }

        private void OnChangeTextCommand()
        {
            Text = "Test";
        }

    }
}
