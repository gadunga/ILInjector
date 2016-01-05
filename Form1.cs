using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics; 
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace Demo_Launcher
{
	public partial class Form1 : Form
	{
		private string exePath = "";
		private string dllPath = "";

		private Process childProcess = null;

		public Form1()
		{
			InitializeComponent();
		}

		private void NormalBtn_Click(object sender, EventArgs e)
		{
			if (string.IsNullOrEmpty(exePath))
			{
				return;
			}

			if (childProcess == null || childProcess.HasExited)
			{
				if (childProcess != null)
				{
					childProcess.Dispose();
				}

				childProcess = LaunchProcess(exePath, false);
			}
		}

		private void InjectBtn_Click(object sender, EventArgs e)
		{
			if (string.IsNullOrEmpty(exePath))
			{
				return;
			}

			if (childProcess == null || childProcess.HasExited)
			{
				if (childProcess != null)
				{
					childProcess.Dispose();
				}

				childProcess = LaunchProcess(exePath, true);
			}
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			if (!string.IsNullOrEmpty(Properties.Settings.Default.exePath))
			{
				exePath = Properties.Settings.Default.exePath;
				exePathTxtBx.Text = exePath;
			}

			if (!string.IsNullOrEmpty(Properties.Settings.Default.dllPath))
			{
				dllPath = Properties.Settings.Default.dllPath;
				dllPathTxtBx.Text = dllPath;
			}
		}

		private void fileSelectorBtn_Click(object sender, EventArgs e)
		{
			if (DialogResult.OK == exeSelectorDialog.ShowDialog())
			{
				exePath = exeSelectorDialog.FileName;
				exePathTxtBx.Text = exePath;
			}
		}

		private void dllSelectorBtn_Click(object sender, EventArgs e)
		{
			if (DialogResult.OK == hookDLLSelector.ShowDialog())
			{
				dllPath = hookDLLSelector.FileName;
				dllPathTxtBx.Text = dllPath;
			}
		}

		private void Form1_FormClosing(object sender, FormClosingEventArgs e)
		{
			Properties.Settings.Default.exePath = exePath;
			Properties.Settings.Default.dllPath = dllPath;
			Properties.Settings.Default.Save();
		}

		private Process LaunchProcess(string fileName, bool hook)
		{
			Process myProcess = new Process();

			try
			{
				myProcess.StartInfo.UseShellExecute = false;
				myProcess.StartInfo.FileName = fileName;

				if (hook && !string.IsNullOrEmpty(dllPath))
				{
					myProcess.StartInfo.EnvironmentVariables.Add("COR_ENABLE_PROFILING", "1");
					myProcess.StartInfo.EnvironmentVariables.Add("COR_PROFILER", "{B6F5BF24-1A2A-40F9-A1C8-8CEDA442D1B7}");
					myProcess.StartInfo.EnvironmentVariables.Add("COR_PROFILER_PATH", dllPath);
				}
				myProcess.Start();
			}
			catch (Exception e)
			{
				Console.WriteLine(e.Message);
				myProcess.Dispose();
				return null;
			}

			return myProcess;
		}
	}
}
