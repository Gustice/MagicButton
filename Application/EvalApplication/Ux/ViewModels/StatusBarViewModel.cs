using EvalApplication.Model;
using Prism.Mvvm;
using System.Threading;
using System.Threading.Tasks;

namespace EvalApplication.Ux.ViewModels
{
    public class StatusBarViewModel : BindableBase
    {
        private Note _statusNote;

        public Note StatusNote
        {
            get => _statusNote;
            set => SetProperty(ref _statusNote, value);
        }

        CancellationTokenSource _propmtToken = new CancellationTokenSource();


        async void StartupDemo()
        {
            await Task.Delay(System.TimeSpan.FromSeconds(2));
            PromptMessage(new Note(Note.NoteType.Info, "Startup Successful"));
        }

        public StatusBarViewModel()
        {
            StartupDemo();
        }

        public async void PromptMessage(Note newNote, uint time = 3)
        {
            _propmtToken.Cancel();

            _propmtToken = new CancellationTokenSource();

            try
            {
                StatusNote = newNote;
                await Task.Delay(System.TimeSpan.FromSeconds(time));
                
                if (_propmtToken.Token.IsCancellationRequested)
                    return;

                StatusNote = new Note();
            }
            catch (System.Exception)
            {
                // Todo: Log or something
            }
        }
    }
}
