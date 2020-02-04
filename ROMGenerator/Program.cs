using System;
using System.IO;
using System.Text;

namespace ROMGenerator
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length != 2)
            {
                Console.WriteLine("USAGE: ROMGenerator <ROM bin file> <Header file>");
                return;
            }

            if (!File.Exists(args[0]))
            {
                Console.WriteLine("ROM file not found");
                return;
            }

            if (File.Exists(args[1]))
            {
                Console.WriteLine("Header file already exists, overwrite?");
                if (Console.ReadKey().Key != ConsoleKey.Y)
                    return;
                Console.WriteLine();
            }

            StreamWriter output = new StreamWriter(File.Create(args[1]));
            Stream input = File.OpenRead(args[0]);

            output.WriteLine("#include \"Arduino.h\"");
            output.WriteLine("");
            output.WriteLine("const PROGMEM byte ROMData[] =");

            output.WriteLine("{");

            byte[] data = new byte[16];

            while (input.Position < input.Length)
            {
                int read = input.Read(data, 0, data.Length);

                StringBuilder line = new StringBuilder("    ");
                
                for (int buc = 0; buc < read; buc++)
                    line.Append("0x" + data[buc].ToString("X2") + ", ");

                if (input.Position == input.Length)
                    line.Remove(line.Length - 2, 2);

                output.WriteLine(line);
            }

            output.WriteLine("};");

            output.Close();
            input.Close();

            Console.WriteLine("Header created succesfully");
        }
    }
}
