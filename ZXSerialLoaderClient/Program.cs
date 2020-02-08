using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;

namespace ZXSerialLoaderClient
{
    class Program
    {
        static void Main(string[] args)
        {
            if(args.Length != 2)
            {
                Console.WriteLine("Usage: ZXSerialLoaderClient <COM PORT> <HEX FILE>");
                return;
            }

            SerialPort sp;
            try
            {
                sp = new SerialPort(args[0], 115200, Parity.None, 8, StopBits.One);
                sp.Open();
            }
            catch 
            {
                Console.WriteLine("Error opening serial port");
                return;
            }

            StreamReader sr;

            try
            {
                sr = File.OpenText(args[1]);
            }
            catch 
            {
                sp.Close();
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

            Console.WriteLine($"File parsed, start address: {startAddress}, end address: {endAddress - 1}");
            Console.WriteLine("Press return to send");
            Console.ReadKey();
            Console.WriteLine("Sending...");
            
            int x = Console.CursorLeft;
            int y = Console.CursorTop;

            Console.WriteLine("0%");

            int totalLength = endAddress - startAddress;
            double pct = 102400.0 / totalLength;
            double currentPct = 0;

            int pos = startAddress;

            bool finished = false;

            while (pos < endAddress)
            {
                int segLen = Math.Min(1024, endAddress - pos);

                byte[] tmpBuffer = new byte[2];

                if (pos + segLen >= endAddress) //Is last segment?
                {
                    finished = true;
                    tmpBuffer[0] = 1;
                }

                sp.Write(tmpBuffer, 0, 1);  //Send last segment

                tmpBuffer[0] =  (byte)(pos & 0xFF);
                tmpBuffer[1] =  (byte)((pos >> 8) & 0xFF);
                sp.Write(tmpBuffer, 0, 2); //Send segment address


                tmpBuffer[0] =  (byte)(segLen & 0xFF);
                tmpBuffer[1] =  (byte)((segLen >> 8) & 0xFF); 
                sp.Write(tmpBuffer, 0, 2); //Send segment size

                while (sp.ReadLine() != "OK") ; //Wait for aknowledge

                //Send segment
                sp.Write(buffer, pos, segLen);


                //Wait for acknowledge
                if(!finished)
                    while (sp.ReadLine() != "NEXT") ;

                currentPct += pct;
                
                if (currentPct > 100)
                    currentPct = 100;

                Console.CursorLeft = x;
                Console.CursorTop = y;

                Console.WriteLine($"{(int)currentPct}%");

                pos += segLen;
            }

            sp.ReadLine();
            Console.WriteLine("Waiting execution...");
            sp.ReadLine();
            Console.WriteLine("Transferred successfully");

            sp.Close();
            sp.Dispose();
            sr.Close();
            sr.Dispose();
        }
    }
}
