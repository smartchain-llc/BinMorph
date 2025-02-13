using System;
using System.Runtime.InteropServices;
using Gtk;

using BinMorph;

class Program
{

    [StructLayout(LayoutKind.Sequential)]
    public struct FieldAttribute{
        public ulong _length;
        public IntPtr _name;
        public IntPtr _endian;

    };

    public class Field{
        public Field(string json){
            IntPtr faPtr = bm_fa_alloc(json);
            _c_Field = Marshal.PtrToStructure<FieldAttribute>(faPtr);
        }
        public string Name{
            get => Marshal.PtrToStringAnsi(_c_Field._name);
        }
        public string Endian{
            get => Marshal.PtrToStringAnsi(_c_Field._endian);
        }
        public ulong Length{
            get => _c_Field._length;
        }
        public void Delete(){
            bm_fa_free(ref _c_Field);
        }
        private FieldAttribute _c_Field;
    }

[DllImport("binmorphui", CallingConvention = CallingConvention.Cdecl)]
public static extern IntPtr to_json(string str);

[DllImport("binmorphui", CallingConvention = CallingConvention.Cdecl)]
public static extern FieldAttribute bm_fa_create(string str);

[DllImport("binmorphui", CallingConvention = CallingConvention.Cdecl)]
public static extern IntPtr bm_fa_alloc(string json);


[DllImport("binmorphui", CallingConvention = CallingConvention.Cdecl)]
public static extern void bm_fa_free(ref FieldAttribute fa);

    static void Main()
    {
        string jsonRaw = "{\"length\":16, \"name\":\"csharptest\", \"endian\":\"big\"}";

        Field fa = new Field(jsonRaw);
        Console.WriteLine($"Field = {fa.Name}, {fa.Endian}, {fa.Length}");
        // external_app_test();
    }


}
