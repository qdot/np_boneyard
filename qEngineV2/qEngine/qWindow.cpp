#include "qWindow.h"
#define WIN32_MEAN_AND_LEAN
#define WIN32_EXTRA_LEAN

namespace qEngine
{

	/************************************ qWindow::qWindow *************************************
	description:
	Constructor

	@spConsole[in]:
	A console object that has already been instantiated by a factory.

	access: public
	------------------------------------------------------------------------------------------
	created: 12.09.02 - 00:31:19
	creator: qDot
	*****************************************************************************************/

	qWindow::qWindow(boost::shared_ptr< qConsole > spConsole) : m_hWnd(0), m_eStatus(WIN_NOT_CREATED), m_spConsole(spConsole)
	{

	}

	/*********************************** qWindow::~qWindow *************************************
	description:
	Destructor
	Kills the window

	access: public
	------------------------------------------------------------------------------------------
	created: 12.09.02 - 00:31:45
	creator: qDot
	*****************************************************************************************/

	qWindow::~qWindow()
	{
		m_spConsole->setMessage(MSG_SHUTDOWN, "qWindow", "Destructor : Destructor Called.");	
		shutdown();
	}

	/********************************** qWindow::shutdown ************************************
	description:
	Shuts down the window.

	access: public
	------------------------------------------------------------------------------------------
	created: 24.10.02 - 00:01:07
	creator: qDot
	*****************************************************************************************/

	void qWindow::shutdown()
	{
		if(m_eStatus == WIN_ALIVE) DestroyWindow(m_hWnd);
		m_eStatus = WIN_DEAD;
		m_spConsole->setMessage(MSG_SHUTDOWN, "qWindow", "Shutdown : Window destruction complete.");	
	}

	/********************************** qWindow::Initialize ***********************************
	description:
	Runs the create window function, which creates and registers the window. Shows the window
	after it is created.

	value:
	Returns true if window creation succeeds.

	access: public
	------------------------------------------------------------------------------------------
	created: 12.09.02 - 00:33:11
	creator: qDot
	*****************************************************************************************/

	bool qWindow::init()
	{
		addHandler(WM_CLOSE, (UINT)qWindow::onClose);
		addHandler(WM_DESTROY, (UINT)qWindow::onDestroy);
		addHandler(WM_PAINT, (UINT)qWindow::onPaint);
		addHandler(WM_QUIT, (UINT)qWindow::onClose);
		if(!create())
		{
			m_spConsole->setMessage(MSG_ERROR, "qWindow", "Initialize : Cannot create window.");
			return false;
		}
		m_spConsole->setMessage(MSG_INIT, "qWindow", "Initialize : Window initialized.");
		show();
		return true;
	}


	/********************************** qWindow::AddHandler ***********************************
	description:
	Adds a handler function to the function map

	@msg[in]:
	Type of message the handler processes
	@handler[in]:
	Pointer to the handler function that will process the message

	access: public
	------------------------------------------------------------------------------------------
	created: 12.09.02 - 00:34:48
	creator: qDot
	*****************************************************************************************/

	void qWindow::addHandler(UINT msg, UINT handler)
	{
		m_MsgHandlers.insert(pair<UINT, UINT>(msg, handler));
	}

	/******************************** qWindow::RemoveHandler **********************************
	description:
	Removes the current handler function for the message specified

	@msg[in]:
	The message to remove the handler for.

	access: public
	------------------------------------------------------------------------------------------
	created: 12.09.02 - 00:35:29
	creator: qDot
	*****************************************************************************************/

	void qWindow::removeHandler(UINT msg)
	{
		m_MsgHandlers.erase(msg);
	}

	/************************************ qWindow::Create *************************************
	description:
	Registers and creates the window. Failure of this function will cause the program to
	exit.

	value:
	Returns true if window is created properly.

	access: public
	------------------------------------------------------------------------------------------
	created: 12.09.02 - 00:36:19
	creator: qDot
	*****************************************************************************************/

	bool qWindow::create()
	{
		HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
		WNDCLASS wndClass;
		DWORD dwStyle;
		DWORD dwExStyle;
		RECT rWindowRect;
		
		rWindowRect.left = (long)0;
		rWindowRect.right = (long)m_iWindowWidth;
		rWindowRect.top = (long)0;
		rWindowRect.bottom = (long)m_iWindowHeight;
		
		
		if(!m_bWindowedMode)
		{
			dwExStyle = WS_EX_APPWINDOW;
			dwStyle = WS_POPUP;
		}
		else
		{
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			dwStyle = WS_OVERLAPPED, WS_CAPTION, WS_SYSMENU, WS_MINIMIZEBOX, WS_MAXIMIZEBOX;
		}

		wndClass.style = 0;
		wndClass.lpfnWndProc = msgRouter;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;	
		wndClass.hInstance = hInstance;
		wndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = NULL;
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = "qEngine";
		if(!RegisterClass(&wndClass))
		{
			m_spConsole->setMessage(MSG_ERROR, "qWindow", "Create : Cannot register window");
			return false;
		}

		AdjustWindowRectEx(&rWindowRect, dwStyle, FALSE, dwExStyle);
		
		if(!m_bWindowedMode)
		{	
			beginFullScreen();
			m_hWnd = CreateWindowEx(NULL, "qEngine", "qEngine", dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
				0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
				NULL, NULL, hInstance, (void*)this);
		}
		else
		{
			m_hWnd = CreateWindowEx(NULL, "qEngine", "qEngine", dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
				0, 0, m_iWindowWidth, m_iWindowHeight, NULL, NULL, hInstance,(void*)this);
		}
		
		if(m_hWnd)
		{
			m_eStatus = WIN_ALIVE;
		}
		else
		{
			m_spConsole->setMessage(MSG_ERROR, "qWindow", "Create : Cannot Create Window");
			m_eStatus = WIN_ERROR;
		}

		m_spConsole->setMessage(MSG_INIT, "qWindow", "Create : Window created and registered");
		return true;
	}

	/******************************** qWindow::HandleMessages *********************************
	description:
		Handles the message pump

	access: public
	------------------------------------------------------------------------------------------
	created: 12.09.02 - 00:37:32
	creator: qDot
	*****************************************************************************************/

	void qWindow::handleMessages() 
	{
		MSG msg;
		if(m_eStatus == WIN_ALIVE)
		{
			while(PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	/********************************** qWindow::MsgRouter ************************************
	description:
	Runs message loop to check for new messages, and processes them according to the
	function mapping

	@hWnd[in]:
	Handle to the current window
	@msg[in]:
	Message to parse
	@wParam[in]:

	@lParam[in]:


	value:
	Returns the process associated with the message

	access: public
	------------------------------------------------------------------------------------------
	created: 12.09.02 - 00:39:02
	creator: qDot
	*****************************************************************************************/

	LRESULT CALLBACK qWindow::msgRouter(HWND hWnd, UINT msg, WPARAM wParam, 
									LPARAM lParam)
	{
		qWindow *wnd = 0;
		unsigned long ulHandler = 0;
		map<UINT, UINT>::iterator p;

		if(msg == WM_NCCREATE)
		{
			wnd = (qWindow *)((LPCREATESTRUCT)lParam)->lpCreateParams;
			SetWindowLong(hWnd, GWL_USERDATA, (long)wnd);
			wnd->m_hWnd = hWnd;
		}
		else
		{
			wnd = (qWindow *)GetWindowLong(hWnd, GWL_USERDATA);
		}

		if(wnd)
		{
			p = wnd->m_MsgHandlers.find(msg);
			if(p != wnd->m_MsgHandlers.end()) ulHandler = p->second;
			if(ulHandler)
			{
				return ((LRESULT (*)(qWindow &, HWND, WPARAM, LPARAM))
					ulHandler)(*wnd, hWnd, wParam, lParam);
			}
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	/*********************************** qWindow::onClose *************************************
	description:
	Function that deals with WM_CLOSE message

	@wnd[in]:

	@hWnd[in]:

	@wParam[in]:

	@lParam[in]:


	access: public
	------------------------------------------------------------------------------------------
	created: 12.09.02 - 00:39:51
	creator: qDot
	*****************************************************************************************/

	void qWindow::onClose(qWindow &wnd, HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		wnd.setDead();
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)NULL);
		DestroyWindow(hWnd);
		PostQuitMessage(0);
	}

	/********************************** qWindow::onDestroy ************************************
	description:
	Function that deals with the WM_DESTROY message

	@wnd[in]:

	@hWnd[in]:

	@wParam[in]:

	@lParam[in]:


	access: public
	------------------------------------------------------------------------------------------
	created: 12.09.02 - 00:40:28
	creator: qDot
	*****************************************************************************************/

	void qWindow::onDestroy(qWindow &wnd, HWND hWnd, WPARAM wParam, LPARAM 
						lParam)
	{
		wnd.setDead();
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)NULL);
		DestroyWindow(hWnd);
		PostQuitMessage(0);
	}

	/*********************************** qWindow::onPaint *************************************
	description:
	Function that deals with the WM_PAINT message

	@wnd[out]:

	@hWnd[in]:

	@wParam[in]:

	@lParam[in]:


	access: public
	------------------------------------------------------------------------------------------
	created: 12.09.02 - 00:40:50
	creator: qDot
	*****************************************************************************************/

	void qWindow::onPaint(qWindow &wnd, HWND hWnd, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}

	/************************************* qWindow::Show **************************************
	description:
	Sets the window to visible

	access: public
	------------------------------------------------------------------------------------------
	created: 12.09.02 - 00:42:14
	creator: qDot
	*****************************************************************************************/

	void qWindow::show()  
	{ 
		if(m_eStatus == WIN_ALIVE)
		{
			ShowWindow(m_hWnd, SW_SHOW);
			UpdateWindow(m_hWnd);
		}
		m_spConsole->setMessage(MSG_COMMON, "qWindow", "Show : Window show called");
	}

	/************************************* qWindow::Hide **************************************
	description:
	Hides the window from view

	access: public
	------------------------------------------------------------------------------------------
	created: 12.09.02 - 00:42:31
	creator: qDot
	*****************************************************************************************/

	void qWindow::hide()  
	{ 
		if(m_eStatus == WIN_ALIVE) ShowWindow(m_hWnd, SW_HIDE);
		m_spConsole->setMessage(MSG_COMMON, "qWindow", "Hide : Window hide called");
	}

	/******************************* qWindow::BeginFullScreen *********************************
	description:
	Go to full screen width X height resolution and save original state

	access: public
	------------------------------------------------------------------------------------------
	created: 12.09.02 - 00:36:41
	creator: qDot
	*****************************************************************************************/

	void qWindow::beginFullScreen()
	{

		// Save current display state
//		m_iWindowPrevWidth = GetSystemMetrics(SM_CXSCREEN);
//		m_iWindowPrevHeight = GetSystemMetrics(SM_CYSCREEN);
		
		// Set new display state
		DEVMODE dm;
		memset(&dm, 0, sizeof(dm));
		dm.dmSize	= sizeof(DEVMODE);
		dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;
		dm.dmPelsWidth	= m_iWindowWidth;
		dm.dmPelsHeight = m_iWindowHeight;
		dm.dmBitsPerPel = m_iWindowBPP;
		dm.dmDisplayFrequency = m_iWindowRefreshRate;
		if(ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			m_spConsole->setMessage(MSG_ERROR, "qWindow", "BeginFullScreen : Window not set, Fullscreen error");
		}
		else
		{
			m_spConsole->setMessage(MSG_COMMON, "qWindow", "BeginFullScreen : Window set to full screen mode");
		}
	}

	/******************************** qWindow::EndFullScreen **********************************
	description:
	Go to desktop setting.

	access: public
	------------------------------------------------------------------------------------------
	created: 12.09.02 - 00:37:02
	creator: qDot
	*****************************************************************************************/

	void qWindow::endFullScreen()
	{
		ChangeDisplaySettings(NULL, 0);
		m_spConsole->setMessage(MSG_COMMON, "qWindow", "EndFullScreen : Window set to desktop mode");
	}

	/***************************** qWindow::getValuesFromConsole ******************************
	description:
	Gets the values for the window settings from the console. If the settings do not exist
	in the console, they are set to a default value and then passed into the console.

	access: public, state: virtual
	------------------------------------------------------------------------------------------
	created: 12.09.02 - 00:43:23
	creator: qDot
	*****************************************************************************************/

	void qWindow::getValuesFromConfigManager(boost::shared_ptr< qConfigManager > spCM)
	{
		//Values to get from Console:
		//WindowWidth - Window Width
		//WindowHeight - Window Height
		//WindowBPP - Window BPP
		//WindowedMode - Fullscreen
		//WindowTitle - Window Title

		m_iWindowWidth = boost::lexical_cast<int>(spCM->getValue("WindowWidth"));
		m_iWindowHeight = boost::lexical_cast<int>(spCM->getValue("WindowHeight"));
		m_iWindowBPP = boost::lexical_cast<int>(spCM->getValue("WindowBPP"));
		m_iWindowRefreshRate = boost::lexical_cast<int>(spCM->getValue("WindowRefreshRate"));
		m_bWindowedMode = boost::lexical_cast<int>(spCM->getValue("WindowedMode"));
		m_strWindowTitle = spCM->getValue("WindowTitle");
	}
}