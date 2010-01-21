//Code yanked from the Window Hider Example on Code Project
//http://www.thecodeproject.com/csharp/windowhider.asp

using System;
using System.Text;
using System.Collections;
using System.Runtime.InteropServices;

namespace Hooky
{
	/// <summary>
	/// Object used to control a Windows Form.
	/// </summary>
	public class Window
	{
		[StructLayout(LayoutKind.Sequential)]
		public struct Win32Consts
		{
			// For use with the INPUT struct, see SendInput for an example
			public const int INPUT_MOUSE     = 0;
			public const int INPUT_KEYBOARD  = 1;
			public const int INPUT_HARDWARE  = 2;
		}   

		[StructLayout(LayoutKind.Sequential)]
		public struct MOUSEINPUT
		{
			public int dx;
			public int dy;
			public int mouseData;
			public int dwFlags;
			public int time;
			public IntPtr dwExtraInfo;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct KEYBDINPUT
		{
			public ushort wVk;
			public ushort wScan;
			public int dwFlags;
			public int time;
			public IntPtr dwExtraInfo;
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct HARDWAREINPUT
		{
			public int uMsg;
			public short wParamL;
			public short wParamH;
		}

		[StructLayout(LayoutKind.Explicit)]
		public struct INPUT
		{
			[FieldOffset(0)] public int type;
			[FieldOffset(4)] public MOUSEINPUT mi;
			[FieldOffset(4)] public KEYBDINPUT ki;
			[FieldOffset(4)] public HARDWAREINPUT hi;
		}

		//declare consts for mouse messages
		public const int MOUSEEVENTF_LEFTDOWN = 0x02;
		public const int MOUSEEVENTF_LEFTUP = 0x04;
		public const int MOUSEEVENTF_RIGHTDOWN = 0x08;
		public const int MOUSEEVENTF_RIGHTUP = 0x10;

		//declare consts for key scan codes
		public const byte VK_TAB = 0x09;
		public const byte VK_MENU = 0x12; // VK_MENU is Microsoft talk for the ALT key
		public const int KEYEVENTF_EXTENDEDKEY = 0x01;
		public const int KEYEVENTF_KEYUP = 0x02;

		/// <summary>
		/// Win32 API Imports
		/// </summary>
		[DllImport("user32.dll")] private static extern IntPtr GetWindowThreadProcessId(IntPtr hWnd, IntPtr ProcessId);
		[DllImport("user32.dll")] private static extern IntPtr AttachThreadInput(IntPtr idAttach, IntPtr idAttachTo, int fAttach);
		[DllImport("user32.dll")] private static extern int SendInput( int cInputs, ref INPUT pInputs, int cbSize );
		[DllImport("user32.dll")] private static extern IntPtr GetForegroundWindow();
		[DllImport("user32.dll")] private static extern bool SetForegroundWindow(IntPtr hWnd);
		 [DllImport("user32", EntryPoint="VkKeyScan")] public static extern short VkKeyScanA(byte cChar_Renamed);


		/// <summary>
		/// Win32 API Constants for ShowWindowAsync()
		/// </summary>
		private const int SW_HIDE = 0;
		private const int SW_SHOWNORMAL = 1;
		private const int SW_SHOWMINIMIZED = 2;
		private const int SW_SHOWMAXIMIZED = 3;
		private const int SW_SHOWNOACTIVATE = 4;
		private const int SW_RESTORE = 9;
		private const int SW_SHOWDEFAULT = 10;

		/// <summary>
		/// Private Fields
		/// </summary>
		private IntPtr m_hWnd;
		private string m_Title;
		private bool m_Visible = true;
		private string m_Process;
		private bool m_WasMax = false;

		/// <summary>
		/// Window Object's Public Properties
		/// </summary>
		public IntPtr hWnd
		{
			get{return m_hWnd;}
		}
		public string Title
		{
			get{return m_Title;}
		}
		public string Process
		{
			get{return m_Process;}
		}

		/// <summary>
		/// Constructs a Window Object
		/// </summary>
		/// <param name="Title">Title Caption</param>
		/// <param name="hWnd">Handle</param>
		/// <param name="Process">Owning Process</param>
		public Window(string Title, IntPtr hWnd, string Process)
		{
			m_Title = Title;
			m_hWnd = hWnd;
			m_Process = Process;
		}

		//Override ToString() 
		public override string ToString()
		{
			//return the title if it has one, if not return the process name
			if (m_Title.Length > 0)
			{
				return m_Title;
			}
			else
			{
				return m_Process;
			}
		}

		public void SendKeyCode(Char charCode)
		{
			SendVKCode((ushort)VkKeyScanA((byte)charCode));
		}

		public void SendKeyDown(Char charCode)
		{
			SendVKDown((ushort)VkKeyScanA((byte)charCode));
		}

		public void SendKeyUp(char charCode)
		{
			SendVKUp((ushort)VkKeyScanA((byte)charCode));
		}

		public void SendVKUp(ushort scanCode)
		{
			if(GetForegroundWindow() != m_hWnd) SetForegroundWindow(m_hWnd);
			INPUT inputUp = new INPUT();
			inputUp.type = Win32Consts.INPUT_KEYBOARD;;
			inputUp.ki.dwFlags = KEYEVENTF_KEYUP;
			inputUp.ki.wVk = scanCode;
			SendInput( 1, ref inputUp, Marshal.SizeOf( inputUp ) );
		}

		public void SendVKDown(ushort scanCode)
		{
			if(GetForegroundWindow() != m_hWnd) SetForegroundWindow(m_hWnd);
			INPUT inputDown = new INPUT();
			inputDown.type = Win32Consts.INPUT_KEYBOARD;;
			inputDown.ki.dwFlags = 0;
			inputDown.ki.wVk = scanCode;
			SendInput( 1, ref inputDown, Marshal.SizeOf( inputDown ) );
		}

		public void SendVKCode(ushort scanCode)
		{
			SendVKDown(scanCode);
			SendVKUp(scanCode);
		}

		public void AttachWindowThread()
		{
			IntPtr ThreadID1 = GetWindowThreadProcessId(GetForegroundWindow(),IntPtr.Zero);
			IntPtr ThreadID2 = GetWindowThreadProcessId(m_hWnd,IntPtr.Zero);
			AttachThreadInput(ThreadID1,ThreadID2,1);
		}

		public void DetachWindowThread()
		{
			IntPtr ThreadID1 = GetWindowThreadProcessId(GetForegroundWindow(),IntPtr.Zero);
			IntPtr ThreadID2 = GetWindowThreadProcessId(m_hWnd,IntPtr.Zero);
			AttachThreadInput(ThreadID1,ThreadID2,0);
		}
	}

	/// <summary>
	/// Collection used to enumerate Window Objects
	/// </summary>
	public class Windows : IEnumerable, IEnumerator
	{
		/// <summary>
		/// Win32 API Imports
		/// </summary>
		[DllImport("user32.dll")] private static extern int GetWindowText(int hWnd, StringBuilder title, int size);
		[DllImport("user32.dll")] private static extern int GetWindowModuleFileName(int hWnd, StringBuilder title, int size);
		[DllImport("user32.dll")] private static extern int EnumWindows(EnumWindowsProc ewp, int lParam); 
		[DllImport("user32.dll")] private static extern bool IsWindowVisible(int hWnd);

		//delegate used for EnumWindows() callback function
		public delegate bool EnumWindowsProc(int hWnd, int lParam);

		private int m_Position = -1; //holds current index of wndArray, necessary for IEnumerable
		
		ArrayList wndArray = new ArrayList(); //array of windows
		
		//Object's private fields
		private bool m_invisible = false;
		private bool m_notitle = false;

		/// <summary>
		/// Collection Constructor with additional options
		/// </summary>
		/// <param name="Invisible">Include invisible Windows</param>
		/// <param name="Untitled">Include untitled Windows</param>
		public Windows(bool Invisible, bool Untitled)
		{
			m_invisible = Invisible;
			m_notitle = Untitled;

			rescanWindows();
		}
		/// <summary>
		/// Collection Constructor
		/// </summary>
		public Windows()
		{
			rescanWindows();
		}
		//EnumWindows CALLBACK function
		private bool EvalWindow(int hWnd, int lParam)
		{
			if (m_invisible == false && !IsWindowVisible(hWnd))
				return(true);

			StringBuilder title = new StringBuilder(256);
			StringBuilder module = new StringBuilder(256);

			GetWindowModuleFileName(hWnd, module, 256);
			GetWindowText(hWnd, title, 256);

			if (m_notitle == false && title.Length == 0)
				return(true);

			wndArray.Add(new Window(title.ToString(), (IntPtr)hWnd, module.ToString()));

			return(true);
		}
		
		//implement IEnumerable
		public IEnumerator GetEnumerator()
		{
			return (IEnumerator)this;
		}
		//implement IEnumerator
		public bool MoveNext()
		{
			m_Position++;
			if (m_Position < wndArray.Count)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		public void rescanWindows()
		{
			wndArray.Clear();
			//Declare a callback delegate for EnumWindows() API call
			EnumWindowsProc ewp = new EnumWindowsProc(EvalWindow);
			//Enumerate all Windows
			EnumWindows(ewp, 0);
		}
		public void Reset()
		{
			m_Position = -1;
		}
		public object Current
		{
			get
			{
				return wndArray[m_Position];
			}
		}
	}
}
