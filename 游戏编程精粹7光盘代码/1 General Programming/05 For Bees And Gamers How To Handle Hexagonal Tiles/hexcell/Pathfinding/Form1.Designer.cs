namespace Pathfinding
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
			this.btnReset = new System.Windows.Forms.Button();
			this.ImageBox = new System.Windows.Forms.PictureBox();
			this.label1 = new System.Windows.Forms.Label();
			((System.ComponentModel.ISupportInitialize)(this.ImageBox)).BeginInit();
			this.SuspendLayout();
			// 
			// btnReset
			// 
			this.btnReset.Location = new System.Drawing.Point(386, 405);
			this.btnReset.Name = "btnReset";
			this.btnReset.Size = new System.Drawing.Size(130, 32);
			this.btnReset.TabIndex = 0;
			this.btnReset.Text = "Reset";
			this.btnReset.UseVisualStyleBackColor = true;
			this.btnReset.Click += new System.EventHandler(this.btnReset_Click);
			// 
			// ImageBox
			// 
			this.ImageBox.Location = new System.Drawing.Point(12, 12);
			this.ImageBox.Name = "ImageBox";
			this.ImageBox.Size = new System.Drawing.Size(504, 387);
			this.ImageBox.TabIndex = 1;
			this.ImageBox.TabStop = false;
			this.ImageBox.MouseClick += new System.Windows.Forms.MouseEventHandler(this.ImageBox_MouseClick);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(80, 415);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(174, 13);
			this.label1.TabIndex = 2;
			this.label1.Text = "Click to set path start and end point";
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(528, 449);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.ImageBox);
			this.Controls.Add(this.btnReset);
			this.Name = "Form1";
			this.Text = "Form1";
			((System.ComponentModel.ISupportInitialize)(this.ImageBox)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button btnReset;
		private System.Windows.Forms.PictureBox ImageBox;
		private System.Windows.Forms.Label label1;
	}
}

