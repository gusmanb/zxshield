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
        static void Main(string[] args)
        {
            StreamReader sr;

            try
            {
                sr = File.OpenText("ROMRAM.hex");
            }
            catch
            {
                Console.WriteLine("Error opening HEX file");
                return;
            }

            Console.WriteLine("Parsing HEX file...");
            byte[] buffer = new byte[1024 * 48]; //48k buffer

            int startAddress = int.MaxValue;
            int endAddress = int.MinValue;

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

                if (addr < startAddress)
                    startAddress = addr;

                if (addr + len > endAddress)
                    endAddress = addr + len;
            }

            int totalLength = endAddress - startAddress;

            sr.Close();

            StreamWriter sw = File.CreateText("arr.txt");

            int pos = startAddress;

            for (int buc = 0; buc < totalLength; buc++)
            {
                sw.WriteLine($"buffer[{buc}] = 0x{buffer[pos].ToString("X2")};");
                pos++;
            }
            sw.Close();
        }
    }
}
