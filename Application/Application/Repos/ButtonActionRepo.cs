using Application.Models;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;

namespace Application.Repos
{
    public class ButtonActionRepo
    {
        const string appName = "MagicButton";
        const string actionsFileName = "Actions.json";
        string folder = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
        private List<ButtonAction> actions;

        public DirectoryInfo ConfigFolder { get; }
        public FileInfo ActionsFile { get; }

        public ButtonActionRepo()
        {
            var cdPath = Path.Combine(folder, appName);
            Directory.CreateDirectory(cdPath);
            ConfigFolder = new DirectoryInfo(cdPath);

            var cfPath = Path.Combine(folder, appName, actionsFileName);
            ActionsFile = new FileInfo(cfPath);

            ReadActionsFromFile();
        }

        private void ReadActionsFromFile()
        {
            try
            {
                var aStr = File.ReadAllText(ActionsFile.FullName);
                actions = JsonConvert.DeserializeObject<List<ButtonAction>>(aStr);
            }
            catch (Exception e)
            {
                Debug.WriteLine("Cannot read from file. Overwriting file" + e.Message);
                var a = CreateDefaulActions();
                Save(a);
            }
        }

        private List<ButtonAction> CreateDefaulActions()
        {
            var actions = new List<ButtonAction>();
            actions.Add(new ButtonAction() { Name = "Lock Screen", Command = "Rundll32.exe user32.dll,LockWorkStation" });
            actions.Add(new ButtonAction() { Name = "Open Google", Command = "explorer \"https://google.com\"" });
            actions.Add(new ButtonAction() { Name = "Demo Bat", Command = @"Resources\DemoProcess.bat" });

            return actions;
        }

        internal IEnumerable<ButtonAction> GetActions()
        {
            if (actions == null)
                ReadActionsFromFile();

            return actions;
        }

        internal void Save(IEnumerable<ButtonAction> actions)
        {
            var aStr = JsonConvert.SerializeObject(actions, Formatting.Indented);

            try
            {
                File.WriteAllText(ActionsFile.FullName, aStr);
            }
            catch (Exception e)
            {
                Debug.WriteLine("Cannot write actions-file: " + e.Message);
            }
        }
    }
}
