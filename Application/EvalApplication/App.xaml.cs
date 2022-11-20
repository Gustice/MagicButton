using System.Windows;
using Prism.Ioc;
using Prism.Regions;

namespace EvalApplication
{
    /// <summary> Interaction logic for App.xaml </summary>
    public partial class App // : PrismApplication
    {
        // Note 1. Initialize(); will be called first, 5. OnInitialized(); will be called last

        /// <inheritdoc /> // 2. This will be called second
        protected override void RegisterTypes(IContainerRegistry containerRegistry)
        {
            //containerRegistry.RegisterSingleton<ISomeInterface, SomeImplementation>();
        }

        /// <inheritdoc /> // 3. This will be called third
        protected override Window CreateShell() => new MainWindow();

        /// <inheritdoc /> // 4. This will be called fourth
        protected override void InitializeShell(Window shell)
        {
            base.InitializeShell(shell);
            RegisterViews(Container.Resolve<IRegionManager>());
        }
        private void RegisterViews(IRegionManager rm)
        {
        }
    }
}
