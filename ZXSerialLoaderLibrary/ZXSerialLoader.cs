using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace ZXSerialLoaderLibrary
{
    public class ZXSerialLoader
    {
        const int serialSpeed = 1000000;

        public enum ZXSerialLoaderResult
        {
            Success = 0,
            Unsupported = -1,
            FileError = -2,
            SerialPortError = -3,
            UnknownResponse = -4
        }
        public ZXSerialLoaderResult LoadFile(string SerialPort, string FileName, int BlockSize, Action<int> Progress)
        {
            string ext = Path.GetExtension(FileName).ToLower();

            SpectrumFile program;

            switch (ext)
            {
                case ".hex":
                    program = HEXFile.Load(FileName);
                    break;

                case ".sna":
                    program = SNAFile.Load(FileName);
                    break;

                case ".z80":
                    program = Z80File.Load(FileName);
                    break;
                case ".tap":
                    program = TAPFile.Load(FileName);
                    break;
                default:
                    return ZXSerialLoaderResult.Unsupported;
            }

            if (program == null)
                return ZXSerialLoaderResult.FileError;

            try
            {
                string dvResponse;

                using (SerialPort serial = new SerialPort(SerialPort, serialSpeed, Parity.None, 8, StopBits.One))
                {
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

                        if(Progress != null)
                            Progress((int)Math.Min(100, currentPercent));

                        pos += segLen;
                    }

                    if (Progress != null)
                        Progress(1000);
                    
                    Console.WriteLine(serial.ReadLine());
                    if (Progress != null)
                        Progress(2000);

                    Console.WriteLine(serial.ReadLine());
                    if (Progress != null)
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

        public ZXSerialLoaderResult DumpFile(string SerialPort, string FileName, int BlockSize, Action<int> Progress)
        {
            try
            {
                byte[] RAMBuffer = new byte[49152];
                byte[] SNAHeader = new byte[27];

                try
                {
                    string dvResponse;

                    using (SerialPort serial = new SerialPort(SerialPort, serialSpeed, Parity.None, 8, StopBits.One))
                    {
                        serial.Open();

                        serial.ReadTimeout = 5000;
                        serial.WriteTimeout = 5000;

                        serial.Write("D");

                        if ((dvResponse = serial.ReadLine()) != "RDY")
                            return ZXSerialLoaderResult.UnknownResponse;

                        int pos = 16384;
                        int endAddress = 65536;

                        bool finished = false;

                        double passPercent = BlockSize * 100 / (endAddress - pos);
                        double currentPercent = 0;
                        int segLen = 0;

                        while (pos < endAddress)
                        {
                            segLen = Math.Min(BlockSize, endAddress - pos);

                            byte[] tmpBuffer = new byte[2];

                            if (pos + segLen >= endAddress) //Is last segment?
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
                            

                            while (serial.BytesToRead < segLen)
                            {
                                ; ;
                            }

                            serial.Read(RAMBuffer, pos - 16384, segLen);

                            //Wait for acknowledge
                            if (!finished)
                            {
                                if ((dvResponse = serial.ReadLine()) != "NEXT")
                                    return ZXSerialLoaderResult.UnknownResponse;
                            }

                            currentPercent += passPercent;

                            if (Progress != null)
                                Progress((int)Math.Min(100, currentPercent));

                            pos += segLen;
                        }

                        if (Progress != null)
                            Progress(1000);
                        
                        if ((dvResponse = serial.ReadLine()) != "HEADER")
                            return ZXSerialLoaderResult.UnknownResponse;

                        while (serial.BytesToRead < 27) ;
                        serial.Read(SNAHeader, 0, 27);

                        if (Progress != null)
                            Progress(2000);

                        serial.ReadLine();

                        if (Progress != null)
                            Progress(3000);

                        serial.Close();

                    }
                }
                catch
                {
                    return ZXSerialLoaderResult.SerialPortError;
                }

                List<byte> finalBuffer = new List<byte>();
                finalBuffer.AddRange(SNAHeader);
                finalBuffer.AddRange(RAMBuffer);

                File.WriteAllBytes(FileName, finalBuffer.ToArray());


                return ZXSerialLoaderResult.Success;

            }
            catch { return ZXSerialLoaderResult.FileError; }
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
            public static SNAFile Load(string FileName)
            {
                byte[] bytes;

                try
                {
                    bytes = File.ReadAllBytes(FileName);
                }
                catch { return null; }

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
            public static HEXFile Load(string FileName)
            {
                HEXFile file = new HEXFile();

                string[] lines;

                try
                {
                    lines = File.ReadAllLines(FileName);
                }
                catch { return null; }

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
                byte[] data;

                try
                {
                   data = File.ReadAllBytes(FileName);
                }
                catch { return null; }

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

                    compressed = (head->Info1 & 32) != 0;

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
                    compressed = true;

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

        public class TAPFile : SpectrumFile
        {
            public override int StartAddress { get; set; } = int.MaxValue;
            public override int EndAddress { get; set; } = int.MinValue;
            public override byte[] Header { get; set; } = null;
            public override byte[] Data { get; set; } = new byte[64 * 1024];
            public override string Operation => "T";
            public static TAPFile Load(string FileName)
            {
                TAPFile file = new TAPFile();

                byte[] data = File.ReadAllBytes(FileName);

                List<TAPBlock> blocks = new List<TAPBlock>();

                int pos = 0;

                while (pos < data.Length)
                {
                    TAPBlock block = TAPBlock.LoadBlock(data, pos, out pos);

                    if (block == null)
                        return null;

                    blocks.Add(block);
                }

                byte[] basicLoader = null;

                while (blocks.Count > 0)
                {
                    if (blocks.Count < 2)
                        return null;

                    TAPBlock headerBlock = blocks[0];
                    blocks.RemoveAt(0);
                    TAPBlock dataBlock = blocks[0];
                    blocks.RemoveAt(0);

                    if (!headerBlock.IsHeader || dataBlock.IsHeader ||
                        !headerBlock.IsValid || !dataBlock.IsValid)
                        return null;

                    TAPHeader realHeader = headerBlock.GetHeader();
                    byte[] realData = dataBlock.GetData();

                    if (realHeader.Type == TAPSpectrumDataType.Program)
                    {
                        if (basicLoader != null)
                            return null;

                        basicLoader = realData;
                    }
                    else if (realHeader.Type == TAPSpectrumDataType.Code)
                    {
                        if (realHeader.Length != realData.Length)
                            return null;

                        Buffer.BlockCopy(realData, 0, file.Data, realHeader.Param1, realData.Length);

                        if (realHeader.Param1 < file.StartAddress)
                            file.StartAddress = realHeader.Param1;

                        if (realHeader.Param1 + realData.Length > file.EndAddress)
                            file.EndAddress = realHeader.Param1 + realData.Length;
                    }
                    else
                        return null;
                }

                if (basicLoader == null)
                    return null;

                file.Header = FindEntryPoint(basicLoader);

                if (file.Header == null)
                    return null;

                return file;

            }

            private static byte[] FindEntryPoint(byte[] BasicLoader)
            {
                byte[] address = null;

                string program = ParseBasicProgram(BasicLoader);

                Regex regUsr = new Regex("RANDOMIZE USR ([0-9]+)");
                var match = regUsr.Match(program);

                if (match != null)
                    address = BitConverter.GetBytes((ushort)32768);

                return address;
            }

            static string[] extendedChars = new string[] {" ", "▝", "▘", "▀", "▗", "▐", "▚", "▜", "▖", "▞", "▌", "▛", "▄", "▟",
    "▙", "█", "Ⓐ", "Ⓑ", "Ⓒ", "Ⓓ", "Ⓔ", "Ⓕ", "Ⓖ", "Ⓗ", "Ⓘ", "Ⓙ", "Ⓚ",
    "Ⓛ", "Ⓜ", "Ⓝ", "Ⓞ", "Ⓟ", "Ⓠ", "Ⓡ", "Ⓢ", "Ⓣ", "Ⓤ", "RND", "INKEY$",
    "PI", "FN ", "POINT ", "SCREEN$ ", "ATTR ", "AT ", "TAB ", "VAL$ ",
    "CODE ", "VAL ", "LEN ", "SIN ", "COS ", "TAN ", "ASN ", "ACS ", "ATN ",
    "LN ", "EXP ", "INT ", "SQR ", "SGN ", "ABS ", "PEEK ", "IN ", "USR ",
    "STR$ ", "CHR$ ", "NOT ", "BIN ", " OR ", " AND ", "<=", ">=", "<>",
    " LINE ", " THEN ", " TO ", " STEP ", " DEF FN ", " CAT ", " FORMAT ",
    " MOVE ", " ERASE ", " OPEN #", " CLOSE #", " MERGE ", " VERIFY ",
    " BEEP ", " CIRCLE ", " INK ", " PAPER ", " FLASH ", " BRIGHT ",
    " INVERSE ", " OVER ", " OUT ", " LPRINT ", " LLIST ", " STOP ", " READ ",
    " DATA ", " RESTORE ", " NEW ", " BORDER ", " CONTINUE ", " DIM ", " REM ",
    " FOR ", " GO TO ", " GO SUB ", " INPUT ", " LOAD ", " LIST ", " LET ",
    " PAUSE ", " NEXT ", " POKE ", " PRINT ", " PLOT ", " RUN ", " SAVE ",
    " RANDOMIZE ", " IF ", " CLS ", " DRAW ", " CLEAR ", " RETURN ", " COPY " };

            static string ParseBasicProgram(byte[] Program)
            {
                StringBuilder sb = new StringBuilder();

                int pos = 0;

                while (pos + 4 < Program.Length)
                {
                    int lineNumber = BigEndianWord(Program, pos);
                    pos += 2;
                    int lineLength = Word(Program, pos);
                    pos += 2;

                    if (lineLength + pos > Program.Length)
                        break;

                    sb.Append($"{lineNumber} {DecodeBasicLine(Program, pos, lineLength)}");

                    pos += lineLength;
                }

                return sb.ToString();
            }

            private static StringBuilder DecodeBasicLine(byte[] Program, int Start, int Length)
            {
                StringBuilder sb = new StringBuilder();
                int pos = 0;

                while (pos < Length)
                {
                    byte ch = Program[pos + Start];
                    pos++;

                    if (0x10 <= ch && ch <= 0x15)
                        pos++;
                    else if (0x16 <= ch && ch <= 0x17)
                        pos += 2;
                    else if (ch == 0x0E)
                        pos += 5;
                    else
                        sb.Append(ConvertChar(ch, sb.Length > 0 && sb[sb.Length - 1] == ' '));
                }

                return sb;
            }

            private static string ConvertChar(byte Chr, bool NoLeadingSpace)
            {
                if (Chr == 0x0D)
                    return "\n";
                else if (Chr >= 0x80)
                {
                    string res = extendedChars[Chr - 0x80];

                    if (NoLeadingSpace && Chr >= 0xA5 && res[0] == ' ')
                        return res.Substring(1);

                    return res;
                }
                else if (Chr == 0x5E)
                    return "↑";
                else if (Chr == 0x60)
                    return "£";
                else if (Chr == 0x7F)
                    return "©";
                else
                    return new string((char)Chr, 1);
            }

            private static ushort Word(byte[] data, int offset)
            {
                return (ushort)(data[offset] | (data[offset + 1] << 8));
            }
            private static ushort BigEndianWord(byte[] data, int offset)
            {
                return (ushort)(data[offset + 1] | (data[offset] << 8));
            }
            class TAPHeader
            {
                public TAPSpectrumDataType Type;
                public byte[] BinaryName;
                public string Name { get { return BinaryName == null ? null : Encoding.ASCII.GetString(BinaryName); } }
                public ushort Length;
                public ushort Param1;
                public ushort Param2;
                public byte Checksum;
            }

            class TAPBlock
            {
                public ushort Length;
                public byte[] Data;

                public bool IsValid
                {
                    get
                    {
                        byte current = Data[0];

                        for (int buc = 1; buc < Data.Length; buc++)
                            current ^= Data[buc];

                        return current == 0;
                    }
                }

                public bool IsHeader { get { return Length == 19 && Data[0] == 0x00; } }

                public TAPHeader GetHeader()
                {
                    if (!IsHeader || !IsValid)
                        return null;

                    TAPHeader header = new TAPHeader();
                    header.Type = (TAPSpectrumDataType)Data[1];
                    header.BinaryName = new byte[10];
                    Buffer.BlockCopy(Data, 2, header.BinaryName, 0, 10);
                    header.Length = Word(Data, 12);
                    header.Param1 = Word(Data, 14);
                    header.Param2 = Word(Data, 16);
                    header.Checksum = Data[18];
                    return header;
                }

                public byte[] GetData()
                {
                    if (IsHeader || !IsValid)
                        return null;

                    byte[] data = new byte[Data.Length - 2];
                    Buffer.BlockCopy(Data, 1, data, 0, data.Length);
                    return data;
                }

                public static TAPBlock LoadBlock(byte[] FileData, int StartPos, out int EndPos)
                {
                    EndPos = 0;

                    TAPBlock block = new TAPBlock();

                    block.Length = Word(FileData, StartPos);

                    if (block.Length + StartPos + 2 > FileData.Length)
                        return null;

                    block.Data = new byte[block.Length];
                    Buffer.BlockCopy(FileData, StartPos + 2, block.Data, 0, block.Data.Length);

                    EndPos = StartPos + 2 + block.Length;

                    return block;
                }
            }

            enum TAPSpectrumDataType : byte
            {
                Program = 0,
                Number = 1,
                Character = 2,
                Code = 3
            }
        }
    }
}
