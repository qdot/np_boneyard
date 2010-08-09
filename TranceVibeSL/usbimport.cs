#region Using directives

using System;
//using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

#endregion

namespace TranceVibeSL
{
	public class usbimport
	{
	public struct USB_DEVICE_DESCRIPTOR {
		SByte bLength;
		SByte bDescriptorType;
		UInt16 bcdUSB;
		SByte bDeviceClass;
		SByte bDeviceSubClass;
		SByte bDeviceProtocol;
		SByte bMaxPacketSize0;
		UInt16 idVendor;
		UInt16 idProduct;
		UInt16 bcdDevice;
		SByte iManufacturer;
		SByte iProduct;
		SByte iSerialNumber;
		SByte bNumConfigurations;
	}
		public usbimport(){
	}
#region UUSBD Imported Functions
		[DllImport("uusbd.dll")] public static extern Int16 Uusbd_Open();
		[DllImport("uusbd.dll")] public static extern Int16 Uusbd_Open_mask(UInt32 flag, SByte Class, SByte SubClass, UInt16 Vendor, UInt16 Product, Byte bcdDevice);
		[DllImport("uusbd.dll")] public static extern void Uusbd_Close(Int16 husb);
		[DllImport("uusbd.dll")] public static extern Int16 Uusbd_OpenPipe(Int16 husb, SByte interface_num, SByte pipe_num);
		[DllImport("uusbd.dll")] public static extern Int32 Uusbd_Check(Int16 husb);
		[DllImport("uusbd.dll")] public static extern Boolean Uusbd_ResetPipe(HandleRef handle);
		[DllImport("uusbd.dll")] public static extern Boolean Uusbd_ResetDevice(Int16 husb);
		[DllImport("uusbd.dll")] public static extern Boolean Uusbd_GetDeviceDescriptor(Int16 husb, USB_DEVICE_DESCRIPTOR descriptor);
		[DllImport("uusbd.dll")] public static extern Boolean Uusbd_GetConfigurationDescriptor(Int16 husb, Char buf, Int32 siz);
		[DllImport("uusbd.dll")] public static extern Boolean Uusbd_GetStatusEndpoint(Int16 husb, UInt16 endp_num, UInt16 status);
		[DllImport("uusbd.dll")] public static extern Boolean Uusbd_ClassRequest(Int16 husb, Boolean dir_in, SByte recipient, SByte bRequest, UInt16 wValue, UInt16 wIndex, UInt16 wLength, Char data);
		[DllImport("uusbd.dll")] public static extern Boolean Uusbd_VendorRequest(Int16 husb, Boolean dir_in, SByte recipient, SByte bRequest, UInt16 wValue, UInt16 wIndex, UInt16 wLength, Char data);
#endregion
		}
	}