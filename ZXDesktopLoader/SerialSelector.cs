using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ZXDesktopLoader
{
    public partial class SerialSelector : Form
    {
        public string SelectedPort { get; set; }
        public SerialSelector()
        {
            InitializeComponent();
            serialDD.Items.Clear();
            serialDD.Items.AddRange(SerialPort.GetPortNames());
            this.DialogResult = DialogResult.Cancel;
        }

        private void acceptBtn_Click(object sender, EventArgs e)
        {
            if (serialDD.SelectedIndex < 0)
            {
                MessageBox.Show("Select a serial port");
                return;
            }

            SelectedPort = serialDD.SelectedItem.ToString();
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void cancelBtn_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}
