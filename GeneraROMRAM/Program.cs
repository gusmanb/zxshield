using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GeneraROMRAM
{
    class Program
    {
        //entrada, salida, prefijo
        //El programa SIEMPRE epieza en 0
        static void Main(string[] args)
        {
            StreamReader sr;

            if (args.Length != 3)
            {
                Console.WriteLine("USAGE: <Input file> <Output file> <Prefix>");
                return;
            }

            byte[] buffer = new byte[1024 * 48]; //48k buffer
            int endAddress = int.MinValue;
            string prefix = args[2];

            try
            {
                using (sr = File.OpenText(args[0]))
                {
                    Console.WriteLine("Parsing HEX file...");


                    while (!sr.EndOfStream)
                    {
                        string line = sr.ReadLine();
                        string sLen = line.Substring(1, 2);
                        int len = int.Parse(sLen, System.Globalization.NumberStyles.AllowHexSpecifier);
                        string sAddr = line.Substring(3, 4);
                        int addr = int.Parse(sAddr, System.Globalization.NumberStyles.AllowHexSpecifier);
                        string op = line.Substring(7, 2);

                        if (op != "00") //Very basic support, only data records supported (I8HEX)
                            continue;

                        string data = line.Substring(9, len * 2);
                        int currentAddress = addr;
                        for (int buc = 0; buc < len * 2; buc += 2)
                        {
                            buffer[currentAddress] = byte.Parse(data.Substring(buc, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
                            currentAddress++;
                        }

                        if (addr + len > endAddress)
                            endAddress = addr + len;
                    }
                }
            }
            catch 
            {
                Console.WriteLine("Error parsing file, check input file name and format");
                return;
            }

            try
            {
                using (StreamWriter sw = File.CreateText(args[1]))
                {

                    sw.WriteLine("#pragma once");
                    sw.WriteLine();
                    sw.WriteLine("#include \"arduino.h\"");
                    sw.WriteLine();
                    sw.WriteLine($"#define {prefix}_SEGMENT_ADDRESS {endAddress}");
                    sw.WriteLine($"#define {prefix}_EXIT_ADDRESS {endAddress - 1}");
                    sw.WriteLine();
                    sw.WriteLine($"const byte PROGMEM {prefix}Program[] =");
                    sw.WriteLine("{");
                    for (int buc = 0; buc < endAddress; buc++)
                    {
                        if (buc % 16 == 0)
                        {
                            sw.WriteLine();
                            sw.Write("    ");
                        }
                        sw.Write($"0x{buffer[buc].ToString("X2")},");
                    }
                    sw.WriteLine();
                    sw.WriteLine("}");
                }
            }
            catch
            {
                Console.WriteLine("Cannot open output file");
                return;
            }
            
        }
    }
}
