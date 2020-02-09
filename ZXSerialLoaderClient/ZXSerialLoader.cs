using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Runtime.InteropServices;
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

                case ".z80":
                    program = Z80File.Load(FileName);
                    break;

                default:
                    return ZXSerialLoaderResult.Unsupported;
            }

            if (program == null)
                return ZXSerialLoaderResult.FileLoadError;

            try
            {
                string dvResponse;

                using SerialPort serial = new SerialPort(SerialPort, 1000000, Parity.None, 8, StopBits.One);
                serial.Open();

                serial.ReadTimeout = 5000;
                serial.WriteTimeout = 5000;

                serial.Write(program.Operation);

                if ((dvResponse = serial.ReadLine()) != "RDY")
                    return ZXSerialLoaderResult.UnknownResponse;

                if (program.Header != null)
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

        class Z80File : SpectrumFile
        {
            public override int StartAddress { get; set; } = 16384;
            public override int EndAddress { get; set; } = 65536;
            public override byte[] Header { get; set; } = new byte[29];
            public override byte[] Data { get; set; } = new byte[64 * 1024];
            public override string Operation => "Z";
            public static unsafe Z80File Load(string FileName)
            {
                byte[] data = File.ReadAllBytes(FileName);

                Z80File file = new Z80File();
                int headerLength = 30;
                bool compressed = true;

                fixed (byte* bData = data)
                {

                    Z80Header* head = (Z80Header*)&bData[0];
                    
                    if (head->PC == 00)
                    {
                        Z80Header2* head2 = (Z80Header2*)&bData[headerLength];
                        headerLength += head2->Size + 2;
                        head->PC = head2->PC;
                    }

                    compressed = (head->Info1 & 16) != 0;

                    fixed (byte* cData = file.Header)
                    {
                        Z80TransferHeader* th = (Z80TransferHeader*)&cData[0];
                        th->I = head->I;
                        th->HLP = head->HLP;
                        th->DEP = head->DEP;
                        th->BCP = head->BCP;
                        th->AFP = head->AFP;
                        th->HL = head->HL;
                        th->DE = head->DE;
                        th->BC = head->BC;
                        th->IY = head->IY;
                        th->IX = head->IX;
                        th->IFFStatus = head->IFF1;
                        th->R = (byte)((head->R & 0x7F) | ((head->Info1 & 0x80) << 7));
                        th->AF = head->AF;
                        th->SP = head->SP;
                        th->IntMode = (byte)(head->Info2 & 3);
                        th->BorderColor = (byte)((head->Info1 >> 1) & 7);
                        th->PC = head->PC;
                    }
                }

                if (headerLength != 30)
                {

                    int i = headerLength;

                    while (i != data.Length)
                    {
                        var datalen = Word(data, i);
                        var page = GetPage(data[i + 2]);

                        if (page == 0xFFFF)
                            return null;

                        i = i + 3; // skip block header


                        if (datalen == 0xFFFF)
                        {
                            datalen = 16384;
                            compressed = false;
                        }

                        UnpackMem(page, data, i, i + datalen, compressed, file);

                        i += datalen;
                    }
                }
                else
                    UnpackMem(0x4000, data, 30, data.Length, compressed, file);

                return file;
            }

            private static ushort Word(byte[] data, int offset)
            {
                return (ushort)(data[offset] | (data[offset + 1] << 8));
            }
            private static ushort GetPage(byte page)
            {
                switch (page)
                {
                    case 0: return 0; // rom
                    case 4: return 0x8000;
                    case 5: return 0xc000;
                    case 8: return 0x4000;
                    default: return 0xFFFF;
                }
            }

            private static ushort SwapBytes(ushort Value)
            {
                return (ushort)(((Value & 0xFF) << 8) | ((Value & 0xFF00) >> 8));
            }

            private static ushort UnpackMem(ushort offset, byte[] data, int start, int end, bool compressed, Z80File File)
            {
                for (int i = start; i < end; ++i)
                {
                    if (compressed &&
                        data[i + 0] == 0x00 &&
                        data[i + 1] == 0xED &&
                        data[i + 2] == 0xED &&
                        data[i + 3] == 0x00)
                        break;

                    if (data[i] == 0xED && data[i + 1] == 0xED && compressed)
                    {
                        var repeat = data[i + 2];
                        var value = data[i + 3];
                        while (repeat-- > 0)
                        {
                            File.Data[offset++] = value;
                        }

                        i = i + 3;
                    }
                    else
                    {
                        File.Data[offset++] = data[i];
                    }
                }
                return offset;
            }

            [StructLayout(LayoutKind.Explicit)]
            struct Z80Header
            {
                [FieldOffset(0)]
                public ushort AF;
                [FieldOffset(2)]
                public ushort BC;
                [FieldOffset(4)]
                public ushort HL;
                [FieldOffset(6)]
                public ushort PC;
                [FieldOffset(8)]
                public ushort SP;
                [FieldOffset(10)]
                public byte I;
                [FieldOffset(11)]
                public byte R;
                [FieldOffset(12)]
                public byte Info1;
                [FieldOffset(13)]
                public ushort DE;
                [FieldOffset(15)]
                public ushort BCP;
                [FieldOffset(17)]
                public ushort DEP;
                [FieldOffset(19)]
                public ushort HLP;
                [FieldOffset(21)]
                public ushort AFP;
                [FieldOffset(23)]
                public ushort IY;
                [FieldOffset(25)]
                public ushort IX;
                [FieldOffset(27)]
                public byte IFF1;
                [FieldOffset(28)]
                public byte IFF2;
                [FieldOffset(29)]
                public byte Info2;
            }

            [StructLayout(LayoutKind.Sequential)]
            struct Z80Header2
            {
                public ushort Size;
                public ushort PC;
            }

            [StructLayout(LayoutKind.Explicit)]
            struct Z80TransferHeader
            {
                [FieldOffset(0)]
                public byte I;
                [FieldOffset(1)]
                public ushort HLP;
                [FieldOffset(3)]
                public ushort DEP;
                [FieldOffset(5)]
                public ushort BCP;
                [FieldOffset(7)]
                public ushort AFP;
                [FieldOffset(9)]
                public ushort HL;
                [FieldOffset(11)]
                public ushort DE;
                [FieldOffset(13)]
                public ushort BC;
                [FieldOffset(15)]
                public ushort IY;
                [FieldOffset(17)]
                public ushort IX;
                [FieldOffset(19)]
                public byte IFFStatus;
                [FieldOffset(20)]
                public byte R;
                [FieldOffset(21)]
                public ushort AF;
                [FieldOffset(23)]
                public ushort SP;
                [FieldOffset(25)]
                public byte IntMode;
                [FieldOffset(26)]
                public byte BorderColor;
                [FieldOffset(27)]
                public ushort PC;
            }
        }
    }
}
