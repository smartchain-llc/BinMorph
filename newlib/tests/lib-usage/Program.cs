using System.Runtime.InteropServices;


namespace BM{
    public class Registry{
        [DllImport("bm", CallingConvention = CallingConvention.Cdecl)]
        public static extern void register_schema_file(string path);


        [DllImport("bm", CallingConvention = CallingConvention.Cdecl)]
        public static extern void registry_list_files();

        [StructLayout(LayoutKind.Sequential)]
        public struct SchemaId{
            public ulong id;
        } 
        [DllImport("bm", CallingConvention = CallingConvention.Cdecl)]
        public static extern SchemaId register_schema(string path);
    }
}
class Program{
static void Main(){
    string path = "./lib-usage.csproj";
    Console.WriteLine("Sending file to libbm");
    // BM.Registry.register_schema_file(path);
    BM.Registry.SchemaId id = BM.Registry.register_schema(path);

    BM.Registry.registry_list_files();

    Console.WriteLine($"Schema ID: {id.id}");
}    
}
