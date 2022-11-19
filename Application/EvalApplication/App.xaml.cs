﻿using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using EvalApplication.Definitions;
using EvalApplication.Ux.Views;
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
            //rm.RegisterViewWithRegion(RegionNames.ControlRegion, typeof(DeviceControlView));
            //rm.RegisterViewWithRegion(RegionNames.LoggerRegion, typeof(LogControlView));

            //rm.RequestNavigate(RegionNames.ControlRegion, nameof(DeviceControlView));
            //rm.RequestNavigate(RegionNames.LoggerRegion, nameof(LogControlView));
        }
    }
}