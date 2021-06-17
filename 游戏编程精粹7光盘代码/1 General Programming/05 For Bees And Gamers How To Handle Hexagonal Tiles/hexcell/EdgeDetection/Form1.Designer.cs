namespace EdgeDetection
{
	partial class Form1
	{
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		/// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Vom Windows Form-Designer generierter Code

		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		private void InitializeComponent()
		{
			this.ImageBox = new System.Windows.Forms.PictureBox();
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
			this.quitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.laplace4erSquareToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.laplaceSquareToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.laplaceHexToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.statusStrip1 = new System.Windows.Forms.StatusStrip();
			this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
			this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
			((System.ComponentModel.ISupportInitialize)(this.ImageBox)).BeginInit();
			this.menuStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// ImageBox
			// 
			this.ImageBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ImageBox.BackColor = System.Drawing.Color.Black;
			this.ImageBox.Location = new System.Drawing.Point(0, 27);
			this.ImageBox.Name = "ImageBox";
			this.ImageBox.Size = new System.Drawing.Size(851, 482);
			this.ImageBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
			this.ImageBox.TabIndex = 0;
			this.ImageBox.TabStop = false;
			// 
			// menuStrip1
			// 
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(851, 24);
			this.menuStrip1.TabIndex = 1;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.toolStripMenuItem1,
            this.quitToolStripMenuItem});
			this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
			this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
			this.fileToolStripMenuItem.Text = "File";
			// 
			// openToolStripMenuItem
			// 
			this.openToolStripMenuItem.Name = "openToolStripMenuItem";
			this.openToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.openToolStripMenuItem.Text = "Open...";
			this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
			this.saveToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.saveToolStripMenuItem.Text = "Save...";
			this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(149, 6);
			// 
			// quitToolStripMenuItem
			// 
			this.quitToolStripMenuItem.Name = "quitToolStripMenuItem";
			this.quitToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.quitToolStripMenuItem.Text = "Quit";
			this.quitToolStripMenuItem.Click += new System.EventHandler(this.quitToolStripMenuItem_Click);
			// 
			// editToolStripMenuItem
			// 
			this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.laplace4erSquareToolStripMenuItem,
            this.laplaceSquareToolStripMenuItem,
            this.laplaceHexToolStripMenuItem});
			this.editToolStripMenuItem.Name = "editToolStripMenuItem";
			this.editToolStripMenuItem.Size = new System.Drawing.Size(43, 20);
			this.editToolStripMenuItem.Text = "Filter";
			// 
			// laplace4erSquareToolStripMenuItem
			// 
			this.laplace4erSquareToolStripMenuItem.Name = "laplace4erSquareToolStripMenuItem";
			this.laplace4erSquareToolStripMenuItem.Size = new System.Drawing.Size(231, 22);
			this.laplace4erSquareToolStripMenuItem.Text = "Laplace 4 neighbours (Square)";
			this.laplace4erSquareToolStripMenuItem.Click += new System.EventHandler(this.laplace4erSquareToolStripMenuItem_Click);
			// 
			// laplaceSquareToolStripMenuItem
			// 
			this.laplaceSquareToolStripMenuItem.Name = "laplaceSquareToolStripMenuItem";
			this.laplaceSquareToolStripMenuItem.Size = new System.Drawing.Size(231, 22);
			this.laplaceSquareToolStripMenuItem.Text = "Laplace 8 neighbours (Square)";
			this.laplaceSquareToolStripMenuItem.Click += new System.EventHandler(this.laplaceSquareToolStripMenuItem_Click);
			// 
			// laplaceHexToolStripMenuItem
			// 
			this.laplaceHexToolStripMenuItem.Name = "laplaceHexToolStripMenuItem";
			this.laplaceHexToolStripMenuItem.Size = new System.Drawing.Size(231, 22);
			this.laplaceHexToolStripMenuItem.Text = "Laplace (Hex)";
			this.laplaceHexToolStripMenuItem.Click += new System.EventHandler(this.laplaceHexToolStripMenuItem_Click);
			// 
			// statusStrip1
			// 
			this.statusStrip1.Location = new System.Drawing.Point(0, 512);
			this.statusStrip1.Name = "statusStrip1";
			this.statusStrip1.Size = new System.Drawing.Size(851, 22);
			this.statusStrip1.TabIndex = 2;
			this.statusStrip1.Text = "statusStrip1";
			// 
			// openFileDialog1
			// 
			this.openFileDialog1.FileName = "openFileDialog1";
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(851, 534);
			this.Controls.Add(this.statusStrip1);
			this.Controls.Add(this.ImageBox);
			this.Controls.Add(this.menuStrip1);
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "Form1";
			this.Text = "Edge Detection";
			((System.ComponentModel.ISupportInitialize)(this.ImageBox)).EndInit();
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.PictureBox ImageBox;
		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem quitToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem laplaceSquareToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem laplaceHexToolStripMenuItem;
		private System.Windows.Forms.StatusStrip statusStrip1;
		private System.Windows.Forms.OpenFileDialog openFileDialog1;
		private System.Windows.Forms.SaveFileDialog saveFileDialog1;
		private System.Windows.Forms.ToolStripMenuItem laplace4erSquareToolStripMenuItem;
	}
}

