namespace ZXDesktopLoader
{
    partial class ZXLoader
    {
        /// <summary>
        /// Variable del diseñador necesaria.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén usando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben desechar; false en caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido de este método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ZXLoader));
            this.cassetteGif = new ZXDesktopLoader.GIFPlayer();
            this.dumpBtn = new System.Windows.Forms.Button();
            this.programLbl = new System.Windows.Forms.Label();
            this.cassetteGif.SuspendLayout();
            this.SuspendLayout();
            // 
            // cassetteGif
            // 
            this.cassetteGif.AllowDrop = true;
            this.cassetteGif.Controls.Add(this.dumpBtn);
            this.cassetteGif.Controls.Add(this.programLbl);
            this.cassetteGif.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cassetteGif.GIF = ((System.Drawing.Image)(resources.GetObject("cassetteGif.GIF")));
            this.cassetteGif.Location = new System.Drawing.Point(0, 0);
            this.cassetteGif.Name = "cassetteGif";
            this.cassetteGif.Size = new System.Drawing.Size(442, 257);
            this.cassetteGif.TabIndex = 0;
            // 
            // dumpBtn
            // 
            this.dumpBtn.BackColor = System.Drawing.Color.Transparent;
            this.dumpBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.dumpBtn.ForeColor = System.Drawing.Color.DimGray;
            this.dumpBtn.Image = ((System.Drawing.Image)(resources.GetObject("dumpBtn.Image")));
            this.dumpBtn.Location = new System.Drawing.Point(371, 202);
            this.dumpBtn.Name = "dumpBtn";
            this.dumpBtn.Size = new System.Drawing.Size(39, 37);
            this.dumpBtn.TabIndex = 1;
            this.dumpBtn.UseVisualStyleBackColor = false;
            this.dumpBtn.Click += new System.EventHandler(this.dumpBtn_Click);
            // 
            // programLbl
            // 
            this.programLbl.BackColor = System.Drawing.Color.Transparent;
            this.programLbl.Font = new System.Drawing.Font("Cabin Sketch", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.programLbl.ForeColor = System.Drawing.Color.DimGray;
            this.programLbl.Location = new System.Drawing.Point(70, 9);
            this.programLbl.Name = "programLbl";
            this.programLbl.Size = new System.Drawing.Size(246, 40);
            this.programLbl.TabIndex = 0;
            this.programLbl.Text = "label1";
            this.programLbl.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // ZXLoader
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(442, 257);
            this.Controls.Add(this.cassetteGif);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ZXLoader";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "ZX Loader";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.cassetteGif.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private GIFPlayer cassetteGif;
        private System.Windows.Forms.Label programLbl;
        private System.Windows.Forms.Button dumpBtn;
    }
}

