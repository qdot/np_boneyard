using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using CookComputing.XmlRpc;
using Microsoft.DirectX;
using Microsoft.DirectX.DirectInput;
using System.Threading;

namespace FFControl
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class FFControl : System.Windows.Forms.Form
	{
		private Microsoft.DirectX.DirectInput.Device ffActuator;
		private Boolean showFFOnly;
		private int[] forceAxes;
		private EffectObject eo; 
		private Effect constantVibrate;
		private Thread queryThread;
		private System.Windows.Forms.Timer queryTimer;

		//GUI Elements
		private System.Windows.Forms.Label controlLabel;
		private System.Windows.Forms.ComboBox controlSelect;
		private System.Windows.Forms.CheckBox ffOnlyCheckBox;
		private System.Windows.Forms.TrackBar feedbackAmountSlider;
		private System.Windows.Forms.Label lessLabel;
		private System.Windows.Forms.Label moreLabel;
		private System.Windows.Forms.Button quitButton;
		private System.Windows.Forms.Button xmlQueryButton;
		private System.Windows.Forms.TextBox objectGUIDValue;
		private System.Windows.Forms.Label objectGUIDLabel;
		private System.Windows.Forms.Label stringReturnLabel;
		private System.Windows.Forms.Label intReturnLabel;
		private System.Windows.Forms.TextBox stringReturnBox;
		private System.Windows.Forms.TextBox intReturnBox;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public FFControl()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
			PopulateDeviceList();
			queryThread = new Thread(new ThreadStart(this.xmlQuery));
			feedbackAmountSlider.Enabled = false;
			objectGUIDValue.Enabled = false;
			xmlQueryButton.Enabled = false;
			queryTimer = new System.Windows.Forms.Timer();
			queryTimer.Interval = 3100;
			queryTimer.Tick += new System.EventHandler(this.startQueryThread);
			feedbackAmountSlider.Maximum = 100;
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if(eo != null)
			{
				eo.Stop();
			}
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
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
			this.controlLabel = new System.Windows.Forms.Label();
			this.controlSelect = new System.Windows.Forms.ComboBox();
			this.ffOnlyCheckBox = new System.Windows.Forms.CheckBox();
			this.feedbackAmountSlider = new System.Windows.Forms.TrackBar();
			this.lessLabel = new System.Windows.Forms.Label();
			this.moreLabel = new System.Windows.Forms.Label();
			this.quitButton = new System.Windows.Forms.Button();
			this.xmlQueryButton = new System.Windows.Forms.Button();
			this.objectGUIDValue = new System.Windows.Forms.TextBox();
			this.objectGUIDLabel = new System.Windows.Forms.Label();
			this.stringReturnLabel = new System.Windows.Forms.Label();
			this.intReturnLabel = new System.Windows.Forms.Label();
			this.stringReturnBox = new System.Windows.Forms.TextBox();
			this.intReturnBox = new System.Windows.Forms.TextBox();
			((System.ComponentModel.ISupportInitialize)(this.feedbackAmountSlider)).BeginInit();
			this.SuspendLayout();
			// 
			// controlLabel
			// 
			this.controlLabel.Location = new System.Drawing.Point(16, 16);
			this.controlLabel.Name = "controlLabel";
			this.controlLabel.Size = new System.Drawing.Size(64, 16);
			this.controlLabel.TabIndex = 0;
			this.controlLabel.Text = "Controller:";
			// 
			// controlSelect
			// 
			this.controlSelect.Location = new System.Drawing.Point(80, 16);
			this.controlSelect.Name = "controlSelect";
			this.controlSelect.Size = new System.Drawing.Size(120, 21);
			this.controlSelect.TabIndex = 1;
			this.controlSelect.SelectedIndexChanged += new System.EventHandler(this.controlSelect_SelectedIndexChanged);
			// 
			// ffOnlyCheckBox
			// 
			this.ffOnlyCheckBox.Location = new System.Drawing.Point(343, 16);
			this.ffOnlyCheckBox.Name = "ffOnlyCheckBox";
			this.ffOnlyCheckBox.TabIndex = 2;
			this.ffOnlyCheckBox.Text = "List FF Only";
			this.ffOnlyCheckBox.CheckedChanged += new System.EventHandler(this.ffOnlyCheckBox_CheckedChanged);
			// 
			// feedbackAmountSlider
			// 
			this.feedbackAmountSlider.Location = new System.Drawing.Point(80, 112);
			this.feedbackAmountSlider.Name = "feedbackAmountSlider";
			this.feedbackAmountSlider.Size = new System.Drawing.Size(296, 45);
			this.feedbackAmountSlider.TabIndex = 3;
			this.feedbackAmountSlider.ValueChanged += new System.EventHandler(this.feedbackAmountSlider_Scroll);
			this.feedbackAmountSlider.Scroll += new System.EventHandler(this.feedbackAmountSlider_Scroll);
			// 
			// lessLabel
			// 
			this.lessLabel.Location = new System.Drawing.Point(16, 112);
			this.lessLabel.Name = "lessLabel";
			this.lessLabel.Size = new System.Drawing.Size(48, 23);
			this.lessLabel.TabIndex = 4;
			this.lessLabel.Text = "Less";
			// 
			// moreLabel
			// 
			this.moreLabel.Location = new System.Drawing.Point(392, 112);
			this.moreLabel.Name = "moreLabel";
			this.moreLabel.Size = new System.Drawing.Size(48, 23);
			this.moreLabel.TabIndex = 5;
			this.moreLabel.Text = "More";
			// 
			// quitButton
			// 
			this.quitButton.Location = new System.Drawing.Point(368, 168);
			this.quitButton.Name = "quitButton";
			this.quitButton.TabIndex = 10;
			this.quitButton.Text = "Quit";
			this.quitButton.Click += new System.EventHandler(this.quitButton_Click);
			// 
			// xmlQueryButton
			// 
			this.xmlQueryButton.Location = new System.Drawing.Point(16, 168);
			this.xmlQueryButton.Name = "xmlQueryButton";
			this.xmlQueryButton.Size = new System.Drawing.Size(136, 23);
			this.xmlQueryButton.TabIndex = 11;
			this.xmlQueryButton.Text = "Start XML Query Timer";
			this.xmlQueryButton.Click += new System.EventHandler(this.xmlQueryButton_Click);
			// 
			// objectGUIDValue
			// 
			this.objectGUIDValue.Location = new System.Drawing.Point(96, 48);
			this.objectGUIDValue.Name = "objectGUIDValue";
			this.objectGUIDValue.Size = new System.Drawing.Size(336, 20);
			this.objectGUIDValue.TabIndex = 12;
			this.objectGUIDValue.Text = "";
			// 
			// objectGUIDLabel
			// 
			this.objectGUIDLabel.Location = new System.Drawing.Point(16, 48);
			this.objectGUIDLabel.Name = "objectGUIDLabel";
			this.objectGUIDLabel.Size = new System.Drawing.Size(72, 16);
			this.objectGUIDLabel.TabIndex = 13;
			this.objectGUIDLabel.Text = "Object GUID:";
			// 
			// stringReturnLabel
			// 
			this.stringReturnLabel.Location = new System.Drawing.Point(16, 88);
			this.stringReturnLabel.Name = "stringReturnLabel";
			this.stringReturnLabel.Size = new System.Drawing.Size(80, 16);
			this.stringReturnLabel.TabIndex = 14;
			this.stringReturnLabel.Text = "String Return:";
			// 
			// intReturnLabel
			// 
			this.intReturnLabel.Location = new System.Drawing.Point(296, 88);
			this.intReturnLabel.Name = "intReturnLabel";
			this.intReturnLabel.Size = new System.Drawing.Size(64, 16);
			this.intReturnLabel.TabIndex = 15;
			this.intReturnLabel.Text = "Int Return:";
			// 
			// stringReturnBox
			// 
			this.stringReturnBox.Location = new System.Drawing.Point(104, 88);
			this.stringReturnBox.Name = "stringReturnBox";
			this.stringReturnBox.TabIndex = 16;
			this.stringReturnBox.Text = "";
			// 
			// intReturnBox
			// 
			this.intReturnBox.Location = new System.Drawing.Point(360, 88);
			this.intReturnBox.Name = "intReturnBox";
			this.intReturnBox.Size = new System.Drawing.Size(72, 20);
			this.intReturnBox.TabIndex = 17;
			this.intReturnBox.Text = "";
			// 
			// FFControl
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(448, 198);
			this.Controls.Add(this.intReturnBox);
			this.Controls.Add(this.stringReturnBox);
			this.Controls.Add(this.intReturnLabel);
			this.Controls.Add(this.stringReturnLabel);
			this.Controls.Add(this.objectGUIDLabel);
			this.Controls.Add(this.objectGUIDValue);
			this.Controls.Add(this.xmlQueryButton);
			this.Controls.Add(this.quitButton);
			this.Controls.Add(this.moreLabel);
			this.Controls.Add(this.lessLabel);
			this.Controls.Add(this.feedbackAmountSlider);
			this.Controls.Add(this.ffOnlyCheckBox);
			this.Controls.Add(this.controlSelect);
			this.Controls.Add(this.controlLabel);
			this.Name = "FFControl";
			this.Text = "Force Feedback Control";
			((System.ComponentModel.ISupportInitialize)(this.feedbackAmountSlider)).EndInit();
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new FFControl());	
		}


		struct SecondLifeRPCValue
		{
			public string Channel;
			public string StringValue;
			public int IntValue;
		};

		[XmlRpcUrl("http://xmlrpc.secondlife.com/cgi-bin/xmlrpc.cgi")]
		interface ISecondLifeRequest
		{
			[XmlRpcMethod("llRemoteData")]
			//SecondLifeRPCValue llRemoteData(string Channel, string StringValue, int IntValue); 
			SecondLifeRPCValue llRemoteData(SecondLifeRPCValue sendVal);
		}

		private void quitButton_Click(object sender, System.EventArgs e)
		{
			Application.Exit();
		}

		private void PopulateDeviceList()
		{
			//Populate All devices
			controlSelect.Items.Clear();
			if(showFFOnly)
			{
				foreach(DeviceInstance di in Manager.GetDevices(DeviceClass.All, EnumDevicesFlags.ForceFeeback))
				{
					controlSelect.Items.Add(di.InstanceName);
				}
			}
			else
			{
				foreach(DeviceInstance di in Manager.Devices)
				{
					controlSelect.Items.Add(di.InstanceName);
				}
			}

		}

		private void ffOnlyCheckBox_CheckedChanged(object sender, System.EventArgs e)
		{
			if(ffOnlyCheckBox.Checked)
			{
				showFFOnly = true;
			}
			else
			{
				showFFOnly = false;
			}
			PopulateDeviceList();
		}

		private void controlSelect_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			if(ffActuator != null)	ffActuator.Unacquire();
			feedbackAmountSlider.Enabled = false;
			objectGUIDValue.Enabled = false;
			xmlQueryButton.Enabled = false;
			foreach(DeviceInstance di in Manager.GetDevices(DeviceClass.All, EnumDevicesFlags.ForceFeeback))
			{
				if(((string)controlSelect.SelectedItem).CompareTo(di.InstanceName) == 0)
				{
					ffActuator = new Device(di.InstanceGuid);
					if(ffActuator == null)
					{
						throw new Exception("Cannot instantiate joystick");
					}
					//set cooperative level.
					ffActuator.SetCooperativeLevel(
						this,
						CooperativeLevelFlags.Exclusive | CooperativeLevelFlags.Background);

					//Set axis mode absolute.
					ffActuator.Properties.AxisModeAbsolute = true;

					//Acquire joystick for capturing.
					ffActuator.Acquire();

					//Configure axes
					foreach(DeviceObjectInstance doi in ffActuator.Objects)
					{

						//Set axes ranges.
						if((doi.ObjectId & (int)DeviceObjectTypeFlags.Axis) != 0)
						{
							ffActuator.Properties.SetRange(
								ParameterHow.ById,
								doi.ObjectId,
								new InputRange(-5000,5000));
						}

						int[] temp;

						// Get info about first two FF axii on the device
						if ((doi.Flags & (int)ObjectInstanceFlags.Actuator) != 0)
						{
							if (forceAxes != null)
							{
								temp = new int[forceAxes.Length + 1];                   
								forceAxes.CopyTo(temp,0);
								forceAxes = temp;
							}
							else
							{
								forceAxes = new int[1];
							}

							// Store the offset of each axis.
							forceAxes[forceAxes.Length - 1] = doi.Offset;
							if (forceAxes.Length == 2)
							{
								break;
							}

							string path = 
								@"C:\temp\force.ffe";
							EffectList el = null;
							el = ffActuator.GetEffects(path,FileEffectsFlags.ModifyIfNeeded);
							EffectObject feo = null;
							foreach(FileEffect fe in el)
							{
								constantVibrate = fe.EffectStruct;
								feo = new EffectObject(
									  fe.EffectGuid,
									  fe.EffectStruct,
									  ffActuator);
								try
								{
									feo.Download();
								}
								catch(Exception ex)
								{
									throw new Exception("Could not download force feedback effect file.", ex);
								}
								eo = feo;
								break;
							}
							constantVibrate.Constant.Magnitude = 0;
							eo.SetParameters(constantVibrate, EffectParameterFlags.TypeSpecificParams);
							eo.Download();
							eo.Start(1);
							feedbackAmountSlider.Enabled = true;
							objectGUIDValue.Enabled = true;
							xmlQueryButton.Enabled = true;
						}
					}
					break;
				}
			}
		}

		private void feedbackAmountSlider_Scroll(object sender, System.EventArgs e)
		{
			constantVibrate.Constant.Magnitude = (feedbackAmountSlider.Value*(10000/feedbackAmountSlider.Maximum));
			eo.SetParameters(constantVibrate, EffectParameterFlags.TypeSpecificParams);
			eo.Download();
		}

		private void xmlQueryButton_Click(object sender, System.EventArgs e)
		{
			if(queryTimer.Enabled)
			{
				queryTimer.Stop();
			}
			else
			{
				queryTimer.Start();
			}
			
		}

		private void startQueryThread(object sender,EventArgs eArgs)
		{
			if(!queryThread.IsAlive)
			{
				queryThread = null;
				queryThread = new Thread(new ThreadStart(this.xmlQuery));
				Console.WriteLine("ThreadState: " + queryThread.ThreadState);
				queryThread.Start();
			}
		}

		private void xmlQuery()
		{
			if(objectGUIDValue.Text == "")
			{
				return;
			}
			ISecondLifeRequest rpcQuery = (ISecondLifeRequest)XmlRpcProxyGen.Create(typeof(ISecondLifeRequest));

			try
			{
				SecondLifeRPCValue sendVal;
				sendVal.Channel = objectGUIDValue.Text;
				sendVal.StringValue = "Test";
				sendVal.IntValue = 0;
				SecondLifeRPCValue retVal = rpcQuery.llRemoteData(sendVal);
				stringReturnBox.Text = retVal.StringValue;
				intReturnBox.Text = Convert.ToString(retVal.IntValue, 10);
				if(retVal.IntValue >= 0)
				{
					feedbackAmountSlider.Value = retVal.IntValue;
				}
				else
				{
					feedbackAmountSlider.Value = 0;
					queryTimer.Stop();
				}
			}
			catch (XmlRpcFaultException fex)
			{
				Console.WriteLine("Fault Response: {0} {1}", 
					fex.FaultCode, fex.FaultString);
				stringReturnBox.Text = fex.FaultString;
			}

		}

	}
}
