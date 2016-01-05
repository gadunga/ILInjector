namespace Demo_Launcher
{
	partial class Form1
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.NormalBtn = new System.Windows.Forms.Button();
			this.InjectBtn = new System.Windows.Forms.Button();
			this.label1 = new System.Windows.Forms.Label();
			this.exePathTxtBx = new System.Windows.Forms.TextBox();
			this.exeSelectorDialog = new System.Windows.Forms.OpenFileDialog();
			this.fileSelectorBtn = new System.Windows.Forms.Button();
			this.label2 = new System.Windows.Forms.Label();
			this.dllPathTxtBx = new System.Windows.Forms.TextBox();
			this.dllSelectorBtn = new System.Windows.Forms.Button();
			this.hookDLLSelector = new System.Windows.Forms.OpenFileDialog();
			this.SuspendLayout();
			// 
			// NormalBtn
			// 
			this.NormalBtn.Location = new System.Drawing.Point(2, 12);
			this.NormalBtn.Name = "NormalBtn";
			this.NormalBtn.Size = new System.Drawing.Size(122, 23);
			this.NormalBtn.TabIndex = 0;
			this.NormalBtn.Text = "Normal - Start";
			this.NormalBtn.UseVisualStyleBackColor = true;
			this.NormalBtn.Click += new System.EventHandler(this.NormalBtn_Click);
			// 
			// InjectBtn
			// 
			this.InjectBtn.Location = new System.Drawing.Point(2, 41);
			this.InjectBtn.Name = "InjectBtn";
			this.InjectBtn.Size = new System.Drawing.Size(122, 23);
			this.InjectBtn.TabIndex = 1;
			this.InjectBtn.Text = "Injected - Start";
			this.InjectBtn.UseVisualStyleBackColor = true;
			this.InjectBtn.Click += new System.EventHandler(this.InjectBtn_Click);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(142, 17);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(53, 13);
			this.label1.TabIndex = 3;
			this.label1.Text = "EXE Path";
			// 
			// exePathTxtBx
			// 
			this.exePathTxtBx.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.exePathTxtBx.Location = new System.Drawing.Point(198, 14);
			this.exePathTxtBx.Name = "exePathTxtBx";
			this.exePathTxtBx.ReadOnly = true;
			this.exePathTxtBx.Size = new System.Drawing.Size(560, 20);
			this.exePathTxtBx.TabIndex = 4;
			// 
			// exeSelectorDialog
			// 
			this.exeSelectorDialog.DefaultExt = "exe";
			this.exeSelectorDialog.Filter = "Executables (*.exe)|*.exe";
			// 
			// fileSelectorBtn
			// 
			this.fileSelectorBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.fileSelectorBtn.Location = new System.Drawing.Point(764, 11);
			this.fileSelectorBtn.Name = "fileSelectorBtn";
			this.fileSelectorBtn.Size = new System.Drawing.Size(25, 25);
			this.fileSelectorBtn.TabIndex = 5;
			this.fileSelectorBtn.Text = "...";
			this.fileSelectorBtn.UseVisualStyleBackColor = true;
			this.fileSelectorBtn.Click += new System.EventHandler(this.fileSelectorBtn_Click);
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(142, 46);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(52, 13);
			this.label2.TabIndex = 6;
			this.label2.Text = "DLL Path";
			// 
			// dllPathTxtBx
			// 
			this.dllPathTxtBx.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.dllPathTxtBx.Location = new System.Drawing.Point(198, 43);
			this.dllPathTxtBx.Name = "dllPathTxtBx";
			this.dllPathTxtBx.ReadOnly = true;
			this.dllPathTxtBx.Size = new System.Drawing.Size(560, 20);
			this.dllPathTxtBx.TabIndex = 7;
			// 
			// dllSelectorBtn
			// 
			this.dllSelectorBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.dllSelectorBtn.Location = new System.Drawing.Point(764, 40);
			this.dllSelectorBtn.Name = "dllSelectorBtn";
			this.dllSelectorBtn.Size = new System.Drawing.Size(25, 25);
			this.dllSelectorBtn.TabIndex = 8;
			this.dllSelectorBtn.Text = "...";
			this.dllSelectorBtn.UseVisualStyleBackColor = true;
			this.dllSelectorBtn.Click += new System.EventHandler(this.dllSelectorBtn_Click);
			// 
			// hookDLLSelector
			// 
			this.hookDLLSelector.Filter = "DLL (*.dll)|*.dll";
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(801, 77);
			this.Controls.Add(this.dllSelectorBtn);
			this.Controls.Add(this.dllPathTxtBx);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.fileSelectorBtn);
			this.Controls.Add(this.exePathTxtBx);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.InjectBtn);
			this.Controls.Add(this.NormalBtn);
			this.Name = "Form1";
			this.Text = "Demo";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
			this.Load += new System.EventHandler(this.Form1_Load);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button NormalBtn;
		private System.Windows.Forms.Button InjectBtn;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox exePathTxtBx;
		private System.Windows.Forms.OpenFileDialog exeSelectorDialog;
		private System.Windows.Forms.Button fileSelectorBtn;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox dllPathTxtBx;
		private System.Windows.Forms.Button dllSelectorBtn;
		private System.Windows.Forms.OpenFileDialog hookDLLSelector;
	}
}

