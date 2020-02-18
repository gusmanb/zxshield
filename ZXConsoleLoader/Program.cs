using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using ZXSerialLoaderLibrary;

namespace ZXSerialLoaderClient
{
    class Program
    {
        static int x;
        static int y;

        static async Task<int> Main(string[] args)
        {
            if (args.Length != 2)
            {
                Console.WriteLine("Usage: ZXSerialLoaderClient <COM PORT> <FILE>");
                return -10;
            }

            Console.WriteLine("ZXSerialLoaderClient 1.0 by -=El Dr. Gusman=-");
            Console.WriteLine();
            Console.WriteLine("Transferring program " + Path.GetFileName(args[1]) + " on serial port " + args[0]);

            x = Console.CursorLeft;
            y = Console.CursorTop;

            ZXSerialLoader loader = new ZXSerialLoader();

            var result = await Task.Run(() => loader.LoadFile(args[0], args[1], 2048, new Action<int>((progress) =>
            {
                Console.CursorLeft = x;
                Console.CursorTop = y;

                int prg = Math.Min(progress, 100);
                int segments = (int)Math.Round(prg / 10.0);

                string consoleMessage = "Progress: [" + new string('=', segments) + new string('·', 10 - segments) + "] " + prg + "%";
                Console.WriteLine(consoleMessage);
                Console.WriteLine();

            })));

            Console.CursorLeft = x;
            Console.CursorTop = y;

            Console.WriteLine();

            switch (result)
            {
                case ZXSerialLoader.ZXSerialLoaderResult.Success:
                    Console.WriteLine("Transfer success");
                    break;
                case ZXSerialLoader.ZXSerialLoaderResult.Unsupported:
                    Console.WriteLine("Unsupported file format");
                    break;
                case ZXSerialLoader.ZXSerialLoaderResult.FileError:
                    Console.WriteLine("Error loading file");
                    break;
                case ZXSerialLoader.ZXSerialLoaderResult.UnknownResponse:
                    Console.WriteLine("Received unknown response from device");
                    break;
                case ZXSerialLoader.ZXSerialLoaderResult.SerialPortError:
                    Console.WriteLine("Serial port error");
                    break;
            }

            return (int)result;

        }
        
    }

}
