using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using Microsoft.DirectX;
using Microsoft.DirectX.DirectInput;
using OpenNETCF.IO.Ports;

namespace Hooky
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class HookyMain : System.Windows.Forms.Form
	{
		private Microsoft.DirectX.DirectInput.Device hookyStick;

		private System.Windows.Forms.Label threadDetectLabel;
		private System.Windows.Forms.Button threadRescanButton;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.ComboBox controlSelect;
		private System.Windows.Forms.Label controlSelectLabel;
		private System.Windows.Forms.Button deviceSelectButton;
		private System.Windows.Forms.Button quitButton;
		private System.Windows.Forms.CheckBox threadHookBox;
		private System.Windows.Forms.Timer updateTimer;
		public System.Windows.Forms.Timer controlXOffTimer;
		public System.Windows.Forms.Timer controlYOffTimer;
		private Boolean yNegState = false;
		private Boolean xNegState = false;
		private Boolean yPosState = false;
		private Boolean xPosState = false;
		private Windows windowContainer;
		private Window secondLifeWindow;
		private System.Windows.Forms.ComboBox PortSelectBox;
		private System.Windows.Forms.Button PortSelectButton;
		private System.Windows.Forms.Label label1;
		private SerialPort port;

		public HookyMain()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
			windowContainer = new Windows(true, false);
			PopulateDeviceList();
			updateTimer = new System.Windows.Forms.Timer();
			controlXOffTimer = new System.Windows.Forms.Timer();
			controlYOffTimer = new System.Windows.Forms.Timer();
			updateTimer.Interval = 200;
			updateTimer.Tick += new System.EventHandler(this.runControlUpdate);
			controlXOffTimer.Tick += new System.EventHandler(this.controlXOff);
			controlYOffTimer.Tick += new System.EventHandler(this.controlYOff);

			string[] ports = SerialPort.GetPortNames();
			foreach( string p in ports ) PortSelectBox.Items.Add(p);
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
				if(port != null) port.Close();
			}
			
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.threadDetectLabel = new System.Windows.Forms.Label();
			this.threadRescanButton = new System.Windows.Forms.Button();
			this.threadHookBox = new System.Windows.Forms.CheckBox();
			this.controlSelectLabel = new System.Windows.Forms.Label();
			this.controlSelect = new System.Windows.Forms.ComboBox();
			this.deviceSelectButton = new System.Windows.Forms.Button();
			this.quitButton = new System.Windows.Forms.Button();
			this.PortSelectBox = new System.Windows.Forms.ComboBox();
			this.label1 = new System.Windows.Forms.Label();
			this.PortSelectButton = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// threadDetectLabel
			// 
			this.threadDetectLabel.Location = new System.Drawing.Point(8, 88);
			this.threadDetectLabel.Name = "threadDetectLabel";
			this.threadDetectLabel.Size = new System.Drawing.Size(192, 23);
			this.threadDetectLabel.TabIndex = 0;
			this.threadDetectLabel.Text = "No Second Life Thread Detected";
			// 
			// threadRescanButton
			// 
			this.threadRescanButton.Enabled = false;
			this.threadRescanButton.Location = new System.Drawing.Point(208, 88);
			this.threadRescanButton.Name = "threadRescanButton";
			this.threadRescanButton.Size = new System.Drawing.Size(136, 23);
			this.threadRescanButton.TabIndex = 1;
			this.threadRescanButton.Text = "Rescan For Thread";
			this.threadRescanButton.Click += new System.EventHandler(this.threadRescanButton_Click);
			// 
			// threadHookBox
			// 
			this.threadHookBox.Enabled = false;
			this.threadHookBox.Location = new System.Drawing.Point(352, 88);
			this.threadHookBox.Name = "threadHookBox";
			this.threadHookBox.Size = new System.Drawing.Size(112, 24);
			this.threadHookBox.TabIndex = 2;
			this.threadHookBox.Text = "Hook To Thread";
			this.threadHookBox.CheckedChanged += new System.EventHandler(this.threadHookBox_CheckedChanged);
			// 
			// controlSelectLabel
			// 
			this.controlSelectLabel.Location = new System.Drawing.Point(8, 24);
			this.controlSelectLabel.Name = "controlSelectLabel";
			this.controlSelectLabel.Size = new System.Drawing.Size(88, 16);
			this.controlSelectLabel.TabIndex = 3;
			this.controlSelectLabel.Text = "Controller Select";
			// 
			// controlSelect
			// 
			this.controlSelect.Location = new System.Drawing.Point(96, 16);
			this.controlSelect.Name = "controlSelect";
			this.controlSelect.Size = new System.Drawing.Size(121, 21);
			this.controlSelect.TabIndex = 4;
			// 
			// deviceSelectButton
			// 
			this.deviceSelectButton.Location = new System.Drawing.Point(224, 16);
			this.deviceSelectButton.Name = "deviceSelectButton";
			this.deviceSelectButton.Size = new System.Drawing.Size(104, 23);
			this.deviceSelectButton.TabIndex = 5;
			this.deviceSelectButton.Text = "Select Device";
			this.deviceSelectButton.Click += new System.EventHandler(this.deviceSelectButton_Click);
			// 
			// quitButton
			// 
			this.quitButton.Location = new System.Drawing.Point(184, 120);
			this.quitButton.Name = "quitButton";
			this.quitButton.TabIndex = 6;
			this.quitButton.Text = "Quit";
			this.quitButton.Click += new System.EventHandler(this.quitButton_Click);
			// 
			// PortSelectBox
			// 
			this.PortSelectBox.Location = new System.Drawing.Point(96, 56);
			this.PortSelectBox.Name = "PortSelectBox";
			this.PortSelectBox.Size = new System.Drawing.Size(121, 21);
			this.PortSelectBox.TabIndex = 7;
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 56);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(72, 16);
			this.label1.TabIndex = 8;
			this.label1.Text = "Serial Port";
			// 
			// PortSelectButton
			// 
			this.PortSelectButton.Location = new System.Drawing.Point(240, 56);
			this.PortSelectButton.Name = "PortSelectButton";
			this.PortSelectButton.TabIndex = 9;
			this.PortSelectButton.Text = "Select Port";
			this.PortSelectButton.Click += new System.EventHandler(this.PortSelectButton_Click);
			// 
			// HookyMain
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(464, 158);
			this.Controls.Add(this.PortSelectButton);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.PortSelectBox);
			this.Controls.Add(this.quitButton);
			this.Controls.Add(this.deviceSelectButton);
			this.Controls.Add(this.controlSelect);
			this.Controls.Add(this.controlSelectLabel);
			this.Controls.Add(this.threadHookBox);
			this.Controls.Add(this.threadRescanButton);
			this.Controls.Add(this.threadDetectLabel);
			this.Name = "HookyMain";
			this.Text = "SL Hooky - By Nonpolynomial Labs";
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new HookyMain());
		}


		private void PopulateDeviceList()
		{
			//Populate All devices
			controlSelect.Items.Clear();
			foreach(
				DeviceInstance di in 
				Manager.GetDevices(
				DeviceClass.GameControl,
				EnumDevicesFlags.AttachedOnly))
			{
				controlSelect.Items.Add(di.InstanceName);
			}
			controlSelect.SelectedIndex = 0;
		}

		private void threadRescanButton_Click(object sender, System.EventArgs e)
		{
			windowContainer.Reset();
			for(IEnumerator itr = windowContainer.GetEnumerator(); itr.MoveNext() != false; )
			{
				System.Diagnostics.Debug.WriteLine(windowContainer.Current.ToString());
				if(windowContainer.Current.ToString().Equals("Second Life"))
				{
					threadDetectLabel.Text = "Second Life Thread is Running";
					threadHookBox.Enabled = true;
					secondLifeWindow = (Window)windowContainer.Current;
					return;
				}
			}
			threadDetectLabel.Text = "Second Life Thread not detected";
		}

		private void quitButton_Click(object sender, System.EventArgs e)
		{
			Application.Exit();
		}

		private void deviceSelectButton_Click(object sender, System.EventArgs e)
		{
			threadRescanButton.Enabled = false;
			threadHookBox.Enabled = false;
			foreach(DeviceInstance di in Manager.GetDevices(
				DeviceClass.GameControl,
				EnumDevicesFlags.AttachedOnly))
			{
				if(((string)controlSelect.SelectedItem).CompareTo(di.InstanceName) == 0)
				{
					hookyStick = new Device(di.InstanceGuid);
					if(hookyStick == null)
					{
						throw new Exception("Cannot instantiate joystick");
					}
					//set cooperative level.
					hookyStick.SetCooperativeLevel(
						this,
						CooperativeLevelFlags.Exclusive | CooperativeLevelFlags.Background);
					System.Diagnostics.Debug.WriteLine("Starting");
					//Set joystick axis ranges.
					foreach(DeviceObjectInstance doi in hookyStick.Objects)
					{
						
						if((doi.ObjectId & (int)DeviceObjectTypeFlags.Axis) != 0)
						{
							hookyStick.Properties.SetRange(
								ParameterHow.ById,
								doi.ObjectId,
								new InputRange(-5000,5000));
						}
						else if((doi.ObjectId & (int)DeviceObjectTypeFlags.AbsoluteAxis) != 0)
						{
							hookyStick.Properties.SetRange(
								ParameterHow.ById,
								doi.ObjectId,
								new InputRange(-5000,5000));
						}
						else
						{
							System.Diagnostics.Debug.WriteLine(doi.ToString());
						}
					}

					//Set joystick axis mode absolute.
					hookyStick.Properties.AxisModeAbsolute = true;
					//Acquire joystick for capturing.
					hookyStick.Acquire();
				}
			}
			threadRescanButton.Enabled = true;

		}

		private void threadHookBox_CheckedChanged(object sender, System.EventArgs e)
		{
			if(threadHookBox.Checked)
			{
				secondLifeWindow.AttachWindowThread();
				updateTimer.Start();
			}
			else
			{
				secondLifeWindow.DetachWindowThread();
				updateTimer.Stop();
			}
			
		}

		private void controlYOff(object sender, System.EventArgs e)
		{
			controlYOffTimer.Enabled = false;
			secondLifeWindow.SendKeyUp('w');
			secondLifeWindow.SendKeyUp('s');
		}
		private void controlXOff(object sender, System.EventArgs e)
		{
			controlXOffTimer.Enabled = false;
			secondLifeWindow.SendKeyUp('a');
			secondLifeWindow.SendKeyUp('d');
		}


		private void runControlUpdate(object sender, System.EventArgs e)
		{
			hookyStick.Poll();
			JoystickState state = hookyStick.CurrentJoystickState;
			if((byte)state.GetButtons().GetValue(0) > 0) threadHookBox.Checked = false;
			int sliderValue = (int)state.GetSlider().GetValue(0);
			if(sliderValue > 500)
			{
				if(sliderValue < 4500)
				{
					controlYOffTimer.Interval = (int)(((float)sliderValue/(float)5000)*100);
					System.Diagnostics.Debug.WriteLine(sliderValue.ToString());
					controlYOffTimer.Enabled = true;
				}
				secondLifeWindow.SendKeyDown('s');
			}
			else
			{
				secondLifeWindow.SendKeyUp('s');
			}
			if (sliderValue < -500)
			{
				if(sliderValue > -4500)
				{
					controlYOffTimer.Interval = (int)(((float)sliderValue/(float)-5000)*100);
					System.Diagnostics.Debug.WriteLine(sliderValue.ToString());
					controlYOffTimer.Enabled = true;
				}
				secondLifeWindow.SendKeyDown('w');
			}
			else
			{
				secondLifeWindow.SendKeyUp('w');
			}
			
			if(state.X > 1000)
			{
				if(state.X < 4500)
				{
					controlXOffTimer.Interval = (int)(((float)state.X/(float)5000)*100);
					System.Diagnostics.Debug.WriteLine("d:" + controlXOffTimer.Interval.ToString());
					controlXOffTimer.Enabled = true;
				}
				secondLifeWindow.SendKeyDown('d');
			}
			else
			{
				secondLifeWindow.SendKeyUp('d');
			}
			if (state.X < -1000)
			{
				if(state.X > -4500)
				{
					controlXOffTimer.Interval = (int)(((float)state.X/(float)-5000)*100);
					System.Diagnostics.Debug.WriteLine("a:" + controlXOffTimer.Interval.ToString());
					controlXOffTimer.Enabled = true;
				}
				secondLifeWindow.SendKeyDown('a');
			}
			else
			{
				secondLifeWindow.SendKeyUp('a');
			}
		}

		private void PortSelectButton_Click(object sender, System.EventArgs e)
		{
			port = new SerialPort((string)PortSelectBox.SelectedItem);
			port.Open();
			port.BaudRate = 38400;
			port.Parity = OpenNETCF.IO.Ports.Parity.None;
			port.StopBits = OpenNETCF.IO.Ports.StopBits.One;
		}

	}
}
