namespace HexLife
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
			this.components = new System.ComponentModel.Container();
			this.Timer = new System.Windows.Forms.Timer(this.components);
			this.ImageBox = new System.Windows.Forms.PictureBox();
			this.ResetButton = new System.Windows.Forms.Button();
			this.speedBar = new System.Windows.Forms.TrackBar();
			this.label1 = new System.Windows.Forms.Label();
			this.rbState = new System.Windows.Forms.RadioButton();
			this.rbBlob = new System.Windows.Forms.RadioButton();
			this.rbActivity = new System.Windows.Forms.RadioButton();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			((System.ComponentModel.ISupportInitialize)(this.ImageBox)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.speedBar)).BeginInit();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// Timer
			// 
			this.Timer.Enabled = true;
			this.Timer.Interval = 40;
			this.Timer.Tick += new System.EventHandler(this.Timer_Tick);
			// 
			// ImageBox
			// 
			this.ImageBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ImageBox.Location = new System.Drawing.Point(12, 12);
			this.ImageBox.Name = "ImageBox";
			this.ImageBox.Size = new System.Drawing.Size(499, 349);
			this.ImageBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
			this.ImageBox.TabIndex = 0;
			this.ImageBox.TabStop = false;
			// 
			// ResetButton
			// 
			this.ResetButton.Location = new System.Drawing.Point(6, 320);
			this.ResetButton.Name = "ResetButton";
			this.ResetButton.Size = new System.Drawing.Size(110, 23);
			this.ResetButton.TabIndex = 1;
			this.ResetButton.Text = "Reset";
			this.ResetButton.UseVisualStyleBackColor = true;
			this.ResetButton.Click += new System.EventHandler(this.ResetButton_Click);
			// 
			// speedBar
			// 
			this.speedBar.Location = new System.Drawing.Point(6, 122);
			this.speedBar.Maximum = 100;
			this.speedBar.Minimum = 10;
			this.speedBar.Name = "speedBar";
			this.speedBar.Size = new System.Drawing.Size(115, 45);
			this.speedBar.TabIndex = 5;
			this.speedBar.TickFrequency = 5;
			this.speedBar.Value = 50;
			this.speedBar.ValueChanged += new System.EventHandler(this.speedValueChanged);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(6, 106);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(38, 13);
			this.label1.TabIndex = 6;
			this.label1.Text = "Speed";
			// 
			// rbState
			// 
			this.rbState.AutoSize = true;
			this.rbState.Checked = true;
			this.rbState.Location = new System.Drawing.Point(6, 19);
			this.rbState.Name = "rbState";
			this.rbState.Size = new System.Drawing.Size(87, 17);
			this.rbState.TabIndex = 7;
			this.rbState.TabStop = true;
			this.rbState.Text = "Current State";
			this.rbState.UseVisualStyleBackColor = true;
			this.rbState.CheckedChanged += new System.EventHandler(this.VisColorButton_Click);
			// 
			// rbBlob
			// 
			this.rbBlob.AutoSize = true;
			this.rbBlob.Location = new System.Drawing.Point(6, 65);
			this.rbBlob.Name = "rbBlob";
			this.rbBlob.Size = new System.Drawing.Size(83, 17);
			this.rbBlob.TabIndex = 8;
			this.rbBlob.Text = "Activity Blob";
			this.rbBlob.UseVisualStyleBackColor = true;
			this.rbBlob.CheckedChanged += new System.EventHandler(this.VisBlobButton_Click);
			// 
			// rbActivity
			// 
			this.rbActivity.AutoSize = true;
			this.rbActivity.Location = new System.Drawing.Point(6, 42);
			this.rbActivity.Name = "rbActivity";
			this.rbActivity.Size = new System.Drawing.Size(59, 17);
			this.rbActivity.TabIndex = 9;
			this.rbActivity.Text = "Activity";
			this.rbActivity.UseVisualStyleBackColor = true;
			this.rbActivity.CheckedChanged += new System.EventHandler(this.VisIntensityButton_Click);
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.ResetButton);
			this.groupBox1.Controls.Add(this.rbActivity);
			this.groupBox1.Controls.Add(this.speedBar);
			this.groupBox1.Controls.Add(this.rbBlob);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Controls.Add(this.rbState);
			this.groupBox1.Location = new System.Drawing.Point(517, 12);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(122, 349);
			this.groupBox1.TabIndex = 10;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Settings";
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(651, 373);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.ImageBox);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Name = "Form1";
			this.Text = "HexLife";
			((System.ComponentModel.ISupportInitialize)(this.ImageBox)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.speedBar)).EndInit();
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Timer Timer;
		private System.Windows.Forms.PictureBox ImageBox;
		private System.Windows.Forms.Button ResetButton;
		private System.Windows.Forms.TrackBar speedBar;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.RadioButton rbBlob;
		private System.Windows.Forms.RadioButton rbActivity;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.RadioButton rbState;
	}
}

