using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Text;
using System.Threading.Tasks;

namespace ZXSerialLoaderClient
{
    public class ZXSerialLoader
    {
        public enum ZXSerialLoaderResult
        {
            Success = 0,
            Unsupported = -1,
            FileLoadError = -2,
            SerialPortError = -3,
            UnknownResponse = -4
        }
        public async Task<ZXSerialLoaderResult> LoadFile(string SerialPort, string FileName, int BlockSize, Action<int> Progress)
        {
            string ext = Path.GetExtension(FileName).ToLower();

            SpectrumFile program;

            switch (ext)
            {
                case ".hex":
                    program = await HEXFile.Load(FileName);
                    break;

                case ".sna":
                    program = await SNAFile.Load(FileName);
                    break;

                default:
                    return ZXSerialLoaderResult.Unsupported;
            }

            if (program == null)
                return ZXSerialLoaderResult.FileLoadError;

            try
            {
                string dvResponse;

                using (SerialPort serial = new SerialPort(SerialPort, 1000000, Parity.None, 8, StopBits.One))
                {
                    serial.Open();
                    
                    serial.ReadTimeout = 5000;
                    serial.WriteTimeout = 5000;

                    serial.Write(program.Operation);

                    if ((dvResponse = serial.ReadLine()) != "RDY")
                        return ZXSerialLoaderResult.UnknownResponse;

                    if(program.Header != null)
                        serial.Write(program.Header, 0, program.Header.Length);

                    int pos = program.StartAddress;
                    bool finished = false;

                    double passPercent = BlockSize * 100 / (program.EndAddress - program.StartAddress);
                    double currentPercent = 0;

                    while (pos < program.EndAddress)
                    {
                        int segLen = Math.Min(BlockSize, program.EndAddress - pos);

                        byte[] tmpBuffer = new byte[2];

                        if (pos + segLen >= program.EndAddress) //Is last segment?
                        {
                            finished = true;
                            tmpBuffer[0] = 1;
                        }

                        serial.Write(tmpBuffer, 0, 1);  //Send last segment

                        tmpBuffer[0] = (byte)(pos & 0xFF);
                        tmpBuffer[1] = (byte)((pos >> 8) & 0xFF);
                        serial.Write(tmpBuffer, 0, 2); //Send segment address


                        tmpBuffer[0] = (byte)(segLen & 0xFF);
                        tmpBuffer[1] = (byte)((segLen >> 8) & 0xFF);
                        serial.Write(tmpBuffer, 0, 2); //Send segment size


                        if ((dvResponse = serial.ReadLine()) != "OK")
                            return ZXSerialLoaderResult.UnknownResponse;

                        //Send segment
                        serial.Write(program.Data, pos, segLen);

                        //Wait for acknowledge
                        if (!finished)
                        {
                            if ((dvResponse = serial.ReadLine()) != "NEXT")
                                return ZXSerialLoaderResult.UnknownResponse;
                        }

                        currentPercent += passPercent;

                        Progress((int)Math.Min(100, currentPercent));

                        pos += segLen;
                    }

                    Progress(1000);
                    serial.ReadLine();
                    Progress(2000);
                    serial.ReadLine();
                    Progress(3000);
                    serial.Close();

                    return ZXSerialLoaderResult.Success;
                }
            }
            catch 
            {
                return ZXSerialLoaderResult.SerialPortError;
            }
        }

        public abstract class SpectrumFile
        {
            public abstract int StartAddress { get; set; }
            public abstract int EndAddress { get; set; }
            public abstract byte[] Header { get; set; }
            public abstract byte[] Data { get; set; }
            public abstract string Operation { get; }
        }
        class SNAFile : SpectrumFile
        {
            public override int StartAddress { get; set; } = 16384;
            public override int EndAddress { get; set; } = 65536;
            public override byte[] Header { get; set; } = new byte[27];
            public override byte[] Data { get; set; } = new byte[64 * 1024];
            public override string Operation => "S";
            public static async Task<SNAFile> Load(string FileName)
            {
                byte[] bytes = await File.ReadAllBytesAsync(FileName);

                if (bytes.Length != (48 * 1024) + 27)
                    return null;

                SNAFile file = new SNAFile();

                Buffer.BlockCopy(bytes, 0, file.Header, 0, 27);
                Buffer.BlockCopy(bytes, 27, file.Data, 16384, 48 * 1024);

                return file;
            }
        }
        class HEXFile : SpectrumFile
        {
            public override int StartAddress { get; set; } = int.MaxValue;
            public override int EndAddress { get; set; } = int.MinValue;
            public override byte[] Header { get; set; } = null;
            public override byte[] Data { get; set; } = new byte[64 * 1024];
            public override string Operation => "H";
            public static async Task<HEXFile> Load(string FileName)
            {
                HEXFile file = new HEXFile();

                string[] lines = await File.ReadAllLinesAsync(FileName);

                foreach (string line in lines)
                {
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
                        file.Data[currentAddress] = byte.Parse(data.Substring(buc, 2), System.Globalization.NumberStyles.AllowHexSpecifier);
                        currentAddress++;
                    }

                    if (addr < file.StartAddress)
                        file.StartAddress = addr;

                    if (addr + len > file.EndAddress)
                        file.EndAddress = addr + len;
                }

                return file;

            }
        }
    }
}
