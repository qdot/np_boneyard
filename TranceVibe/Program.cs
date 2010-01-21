#region Using directives

using System;
//using System.Collections.Generic;
using System.Text;

#endregion

namespace TranceSharp
{
	class Program{
		public static UInt16 vibval;
		static void Main(string[] args)
		{
			usbcontrol.InitUSBHandle();
			usbcontrol.SetPower(0x0);
			/*
			while (1 != 0)	{
				vibval =(0);
				if (vibval == 0) { usbcontrol.CloseUSBHandle(); return;}
				usbcontrol.SetPower(vibval);
			}
			*/
		}
	}
}
