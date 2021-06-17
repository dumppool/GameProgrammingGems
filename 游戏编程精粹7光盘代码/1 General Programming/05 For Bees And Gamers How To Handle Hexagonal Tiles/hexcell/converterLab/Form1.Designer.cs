namespace converterLab
{
	partial class ConverterLabForm
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
			this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.label4 = new System.Windows.Forms.Label();
			this.btnCopnvertToSquare = new System.Windows.Forms.Button();
			this.btnSaveSource = new System.Windows.Forms.Button();
			this.comboBox1 = new System.Windows.Forms.ComboBox();
			this.btnLoadSource = new System.Windows.Forms.Button();
			this.label3 = new System.Windows.Forms.Label();
			this.numCellArea = new System.Windows.Forms.NumericUpDown();
			this.btnConvertToHex = new System.Windows.Forms.Button();
			this.label2 = new System.Windows.Forms.Label();
			this.numGridHeight = new System.Windows.Forms.NumericUpDown();
			this.label1 = new System.Windows.Forms.Label();
			this.numGridWidth = new System.Windows.Forms.NumericUpDown();
			this.picSource = new System.Windows.Forms.PictureBox();
			this.picResult = new System.Windows.Forms.PictureBox();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
			this.label5 = new System.Windows.Forms.Label();
			this.label6 = new System.Windows.Forms.Label();
			this.groupBox1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.numCellArea)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numGridHeight)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numGridWidth)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.picSource)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.picResult)).BeginInit();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			this.SuspendLayout();
			// 
			// openFileDialog
			// 
			this.openFileDialog.FileName = "openFileDialog1";
			// 
			// groupBox1
			// 
			this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)));
			this.groupBox1.Controls.Add(this.label4);
			this.groupBox1.Controls.Add(this.btnCopnvertToSquare);
			this.groupBox1.Controls.Add(this.btnSaveSource);
			this.groupBox1.Controls.Add(this.comboBox1);
			this.groupBox1.Controls.Add(this.btnLoadSource);
			this.groupBox1.Controls.Add(this.label3);
			this.groupBox1.Controls.Add(this.numCellArea);
			this.groupBox1.Controls.Add(this.btnConvertToHex);
			this.groupBox1.Controls.Add(this.label2);
			this.groupBox1.Controls.Add(this.numGridHeight);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Controls.Add(this.numGridWidth);
			this.groupBox1.Location = new System.Drawing.Point(12, 6);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(124, 467);
			this.groupBox1.TabIndex = 8;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Settings";
			this.groupBox1.Enter += new System.EventHandler(this.groupBox1_Enter);
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(6, 152);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(50, 13);
			this.label4.TabIndex = 23;
			this.label4.Text = "Sampling";
			// 
			// btnCopnvertToSquare
			// 
			this.btnCopnvertToSquare.Location = new System.Drawing.Point(6, 48);
			this.btnCopnvertToSquare.Name = "btnCopnvertToSquare";
			this.btnCopnvertToSquare.Size = new System.Drawing.Size(112, 23);
			this.btnCopnvertToSquare.TabIndex = 22;
			this.btnCopnvertToSquare.Text = "Convert to Square";
			this.btnCopnvertToSquare.UseVisualStyleBackColor = true;
			this.btnCopnvertToSquare.Click += new System.EventHandler(this.btnCopnvertToSquare_Click);
			// 
			// btnSaveSource
			// 
			this.btnSaveSource.Location = new System.Drawing.Point(6, 77);
			this.btnSaveSource.Name = "btnSaveSource";
			this.btnSaveSource.Size = new System.Drawing.Size(112, 23);
			this.btnSaveSource.TabIndex = 21;
			this.btnSaveSource.Text = "Save Bitmap..";
			this.btnSaveSource.UseVisualStyleBackColor = true;
			this.btnSaveSource.Click += new System.EventHandler(this.btnSaveSource_Click);
			// 
			// comboBox1
			// 
			this.comboBox1.FormattingEnabled = true;
			this.comboBox1.Items.AddRange(new object[] {
            "Geometric",
            "Linear",
            "Nearest"});
			this.comboBox1.Location = new System.Drawing.Point(6, 168);
			this.comboBox1.Name = "comboBox1";
			this.comboBox1.Size = new System.Drawing.Size(112, 21);
			this.comboBox1.TabIndex = 20;
			this.comboBox1.Text = "Linear";
			// 
			// btnLoadSource
			// 
			this.btnLoadSource.Location = new System.Drawing.Point(6, 106);
			this.btnLoadSource.Name = "btnLoadSource";
			this.btnLoadSource.Size = new System.Drawing.Size(112, 23);
			this.btnLoadSource.TabIndex = 19;
			this.btnLoadSource.Text = "Load Bitmap...";
			this.btnLoadSource.UseVisualStyleBackColor = true;
			this.btnLoadSource.Click += new System.EventHandler(this.btnLoadSource_Click);
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(6, 274);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(47, 13);
			this.label3.TabIndex = 18;
			this.label3.Text = "Cell Size";
			// 
			// numCellArea
			// 
			this.numCellArea.Location = new System.Drawing.Point(6, 290);
			this.numCellArea.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.numCellArea.Name = "numCellArea";
			this.numCellArea.Size = new System.Drawing.Size(112, 20);
			this.numCellArea.TabIndex = 17;
			this.numCellArea.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
			this.numCellArea.ValueChanged += new System.EventHandler(this.numCellArea_ValueChanged);
			// 
			// btnConvertToHex
			// 
			this.btnConvertToHex.Location = new System.Drawing.Point(6, 19);
			this.btnConvertToHex.Name = "btnConvertToHex";
			this.btnConvertToHex.Size = new System.Drawing.Size(112, 23);
			this.btnConvertToHex.TabIndex = 16;
			this.btnConvertToHex.Text = "Convert to Hex";
			this.btnConvertToHex.UseVisualStyleBackColor = true;
			this.btnConvertToHex.Click += new System.EventHandler(this.btnConvertToHex_Click);
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(6, 233);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(60, 13);
			this.label2.TabIndex = 15;
			this.label2.Text = "Grid Height";
			// 
			// numGridHeight
			// 
			this.numGridHeight.Location = new System.Drawing.Point(6, 249);
			this.numGridHeight.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
			this.numGridHeight.Name = "numGridHeight";
			this.numGridHeight.Size = new System.Drawing.Size(112, 20);
			this.numGridHeight.TabIndex = 14;
			this.numGridHeight.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(6, 194);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(57, 13);
			this.label1.TabIndex = 13;
			this.label1.Text = "Grid Width";
			// 
			// numGridWidth
			// 
			this.numGridWidth.Location = new System.Drawing.Point(6, 210);
			this.numGridWidth.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
			this.numGridWidth.Name = "numGridWidth";
			this.numGridWidth.Size = new System.Drawing.Size(112, 20);
			this.numGridWidth.TabIndex = 12;
			this.numGridWidth.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			// 
			// picSource
			// 
			this.picSource.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.picSource.BackColor = System.Drawing.Color.Black;
			this.picSource.Location = new System.Drawing.Point(3, 19);
			this.picSource.Name = "picSource";
			this.picSource.Size = new System.Drawing.Size(274, 445);
			this.picSource.TabIndex = 1;
			this.picSource.TabStop = false;
			// 
			// picResult
			// 
			this.picResult.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.picResult.BackColor = System.Drawing.Color.Black;
			this.picResult.Location = new System.Drawing.Point(3, 19);
			this.picResult.Name = "picResult";
			this.picResult.Size = new System.Drawing.Size(319, 445);
			this.picResult.TabIndex = 7;
			this.picResult.TabStop = false;
			this.picResult.MouseDown += new System.Windows.Forms.MouseEventHandler(this.picResult_MouseDown);
			this.picResult.MouseUp += new System.Windows.Forms.MouseEventHandler(this.picResult_MouseUp);
			// 
			// splitContainer1
			// 
			this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.splitContainer1.Location = new System.Drawing.Point(146, 6);
			this.splitContainer1.Name = "splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.label5);
			this.splitContainer1.Panel1.Controls.Add(this.picSource);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.label6);
			this.splitContainer1.Panel2.Controls.Add(this.picResult);
			this.splitContainer1.Size = new System.Drawing.Size(609, 467);
			this.splitContainer1.SplitterDistance = 280;
			this.splitContainer1.TabIndex = 9;
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(6, 3);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(41, 13);
			this.label5.TabIndex = 10;
			this.label5.Text = "Square";
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(3, 3);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(26, 13);
			this.label6.TabIndex = 11;
			this.label6.Text = "Hex";
			// 
			// ConverterLabForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoSize = true;
			this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.ClientSize = new System.Drawing.Size(767, 485);
			this.Controls.Add(this.splitContainer1);
			this.Controls.Add(this.groupBox1);
			this.Name = "ConverterLabForm";
			this.Text = "ConverterLab";
			this.Load += new System.EventHandler(this.ConverterLabForm_Load);
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.numCellArea)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numGridHeight)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numGridWidth)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.picSource)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.picResult)).EndInit();
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel1.PerformLayout();
			this.splitContainer1.Panel2.ResumeLayout(false);
			this.splitContainer1.Panel2.PerformLayout();
			this.splitContainer1.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.OpenFileDialog openFileDialog;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Button btnLoadSource;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.NumericUpDown numCellArea;
		private System.Windows.Forms.Button btnConvertToHex;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.NumericUpDown numGridHeight;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.NumericUpDown numGridWidth;
		private System.Windows.Forms.PictureBox picSource;
		private System.Windows.Forms.PictureBox picResult;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.ComboBox comboBox1;
		private System.Windows.Forms.Button btnSaveSource;
		private System.Windows.Forms.SaveFileDialog saveFileDialog;
		private System.Windows.Forms.Button btnCopnvertToSquare;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label6;
	}
}

