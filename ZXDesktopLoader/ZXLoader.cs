using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ZXSerialLoaderLibrary;

namespace ZXDesktopLoader
{
    public partial class ZXLoader : Form
    {
        string[] allowedExtensions = new string[] { ".hex", ".sna", ".z80", ".tap" };
        string serial;
        bool transferring = false;
        public ZXLoader()
        {
            InitializeComponent();
            cassetteGif.DragEnter += GifPlayer1_DragEnter;
            cassetteGif.DragDrop += GifPlayer1_DragDrop;
        }

        private async void GifPlayer1_DragDrop(object sender, DragEventArgs e)
        {
            transferring = true;

            try
            {
                if (e.Data.GetDataPresent(DataFormats.FileDrop) && e.Effect == DragDropEffects.Copy)
                {
                    var files = e.Data.GetData(DataFormats.FileDrop) as string[];

                    if (files.Length > 1)
                        return;
                    else
                    {
                        string ext = Path.GetExtension(files[0]).ToLower();

                        if (!allowedExtensions.Contains(ext))
                            return;

                        ZXSerialLoader loader = new ZXSerialLoader();

                        programLbl.Text = Path.GetFileNameWithoutExtension(files[0]);
                        programLbl.Visible = true;
                        cassetteGif.Play();

                        var result = await Task.Run(() => loader.LoadFile(serial, files[0], 2048, null));

                        cassetteGif.Stop();
                        programLbl.Visible = false;

                        if (result != ZXSerialLoader.ZXSerialLoaderResult.Success)
                            MessageBox.Show("Error transferring program");
                        
                    }
                }
            }
            finally { transferring = false; }
        }

        private void GifPlayer1_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop) && !transferring)
            {
                var files = e.Data.GetData(DataFormats.FileDrop) as string[];

                if (files.Length > 1)
                    e.Effect = DragDropEffects.None;
                else
                {
                    string ext = Path.GetExtension(files[0]).ToLower();

                    if (!allowedExtensions.Contains(ext))
                        e.Effect = DragDropEffects.None;
                    else
                        e.Effect = DragDropEffects.Copy;
                }
            }
            else
                e.Effect = DragDropEffects.None;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            using (var sersel = new SerialSelector())
            {
                if (sersel.ShowDialog() != DialogResult.OK)
                {
                    this.Close();
                    Application.Exit();
                }

                serial = sersel.SelectedPort;
            }
        }

        private async void dumpBtn_Click(object sender, EventArgs e)
        {
            string file;

            using (SaveFileDialog dlg = new SaveFileDialog())
            {
                dlg.Filter = "Snapshot file|*.SNA";

                if (dlg.ShowDialog() != DialogResult.OK)
                    return;

                file = dlg.FileName;
            }

            ZXSerialLoader loader = new ZXSerialLoader();

            programLbl.Text = Path.GetFileNameWithoutExtension(file);
            programLbl.Visible = true;
            cassetteGif.Play();

            var result = await Task.Run(() => loader.DumpFile(serial, file, 128, null));

            cassetteGif.Stop();
            programLbl.Visible = false;

            if (result != ZXSerialLoader.ZXSerialLoaderResult.Success)
                MessageBox.Show("Error dumping program");
            
        }
    }
}
