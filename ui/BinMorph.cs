
using System;
using System.Runtime.InteropServices;
using Gtk;

[StructLayout(LayoutKind.Sequential)]
public struct FieldAttribute{
    
}

namespace BinMorph{
public class App {
    private struct UIContents{
        public Window window;

    }
    private static UIContents  _ui;

    private static bool _initialized = false;

    public static void Run(){
        if(!App._initialized)
            throw new Exception("GTK Application has not been initialized");
        
        App._ui.window.ShowAll();

        Application.Run();
    }
    public static void Init(){
        if(App._initialized)
            throw new Exception("GTK Application is already created");

        App.CreateUI();

        App._initialized = true;
    }

    private static void CreateUI(){
        Application.Init();

        App._ui.window = new Window("BinMorph");
        App._ui.window.SetDefaultSize(200,100);
        App._ui.window.DeleteEvent += (o,args) => Application.Quit();

        Fixed frame = [];
        Button btn = new("Use BinMorph");
        btn.SetSizeRequest(75,20);

        btn.Clicked += (sender, e) => {
        };

        frame.Put(btn, 50,50);

        App._ui.window.Add(frame);
    }
}
}