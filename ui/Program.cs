using System;
using System.Runtime.InteropServices;
using Gtk;

using BinMorph;

class Program
{

    [StructLayout(LayoutKind.Sequential)]
    public struct FieldAttribute{
        public ulong length;
        public readonly string name;
        public readonly string endian;
    };

[DllImport("binmorphui", CallingConvention = CallingConvention.Cdecl)]
public static extern IntPtr to_json(string str);

[DllImport("binmorphui", CallingConvention = CallingConvention.Cdecl)]
public static extern void bm_fa_create(string str, ref FieldAttribute field);

    static void Main()
    {
        string jsonRaw = "{\"length\":16, \"name\":\"csharptest\", \"endian\":\"big\"}";
        FieldAttribute fa = new();
        IntPtr faPtr = Marshal.AllocHGlobal(Marshal.SizeOf<FieldAttribute>());

        Marshal.StructureToPtr(fa, faPtr, false);
        bm_fa_create(jsonRaw, ref fa);
        Console.WriteLine($"FieldAttribute => { fa.name }, {fa.endian}, {fa.length}");
        // external_app_test();
    }


}
