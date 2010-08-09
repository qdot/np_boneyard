#region Using directives

using System;
using System.Text;

#endregion

namespace TranceVibeSL
{
	public class usbcontrol
	{
		public static Int16 husb;
		public usbcontrol()
		{
		}
	public static Boolean SetPower(UInt16 val)
		{
			if (husb == -1) return false;
			return usbimport.Uusbd_VendorRequest(husb, false, 1, 1, val, 0, 0, (Char)0);
		}
	public static void CloseUSBHandle()
		{
			if (husb != -1)
			{
				SetPower(0x0000);
				usbimport.Uusbd_Close(husb);
			}
		}
	public static Int16 InitUSBHandle()
		{
			Int32 ret;

			if (husb != -1)
			{
				SetPower(0x00FF);
				usbimport.Uusbd_Close(husb);
			}

			husb = usbimport.Uusbd_Open_mask(1<<2 | 1<<3, 0, 0, 0x0B49, 0x064F, 0);

			if (husb == -1)
			{
				return -1;
			}

			ret = usbimport.Uusbd_Check(husb);

			if (ret != 0)
			{
				usbimport.Uusbd_Close(husb);
				husb = -1;
				return -1;
			}
//Move0.5sec
			SetPower(0x00FF);
			SetPower(0x00EE);

			return husb;
		}
	}
}
