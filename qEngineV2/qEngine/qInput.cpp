#include "qinput.h"

namespace qEngine
{
	
/************************************ qInput::qInput *************************************
description:
   Constructor

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 17:21:10
creator: qDot
*****************************************************************************************/

	qInput::qInput(void)
	{
	}

/************************************* qInput::init **************************************
description:
   Initialization function. Initializes all specified devices and moves the console to the
   internal pointer for the object.

@hWnd[in]:
   A handle that references the current window.
@spConsole[in]:
   A smart pointer to the console already started by the engine.
@dwDevices[in]:
   A bitfield to decide which devices the class will initialize

value:
   Returns true if all specified devices are initialized properly.

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 17:23:08
creator: qDot
*****************************************************************************************/

	bool qInput::init(HWND hWnd, boost::shared_ptr<qConsole> spConsole, boost::shared_ptr<qConfigManager> spCM, DWORD dwDevices)
	{
		m_spConsole = spConsole;
		m_spCM = spCM;
		getValuesFromConfigManager();
		if(dwDevices == 0)
		{
			m_spConsole->setMessage(MSG_ERROR, "qInput", "init : No devices to initialize!");
			return false;
		}
		
		if(FAILED(DirectInput8Create((HINSTANCE)GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL)))
		{
			m_spConsole->setMessage(MSG_ERROR, "qInput", "init : Cannot Create DI8 interface");
			return false;
		}
		
		m_hWnd = hWnd;
		m_spConsole->setMessage(MSG_INIT, "qInput", "init : DI8 Interface created");

		if(dwDevices && Q_KEYBOARD == Q_KEYBOARD)
		{
			m_spConsole->setMessage(MSG_INIT, "qInput", "init : Creating Keyboard Interface");
			if(!initKeyboard())
			{
				m_spConsole->setMessage(MSG_ERROR, "qInput", "init : Cannot create keyboard interface");
				return false;
			}
			m_spConsole->setMessage(MSG_INIT, "qInput", "init : Keyboard Initialized");
		}

		if(dwDevices && Q_MOUSE == Q_MOUSE)
		{
			m_lMouseX = 0;
			m_lMouseY = 0;
			m_spConsole->setMessage(MSG_INIT, "qInput", "init : Creating Mouse Interface");
			if(!initMouse())
			{
				m_spConsole->setMessage(MSG_ERROR, "qInput", "init : Cannot create mouse interface");
				return false;
			}
			m_spConsole->setMessage(MSG_INIT, "qInput", "init : Mouse Initialized");
		}

		return true;
	}

/******************************** qInput::updateDevices **********************************
description:
   Checks to make sure which devices have initialized, and reads the information from
   them.

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 17:27:32
creator: qDot
*****************************************************************************************/

	void qInput::updateDevices()
	{
		if(m_pDIKeyboard)
		{
			readKeyboard();
		}
		if(m_pDIMouse)
		{
			readMouse();
		}
	}

/********************************* qInput::initKeyboard **********************************
description:
   Initializes the keyboard object.

value:
   Returns true if the keyboard has been initialized properly, otherwise returns false.

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 17:28:05
creator: qDot
*****************************************************************************************/

	bool qInput::initKeyboard()
	{
		if (FAILED(m_pDI->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboard, NULL)))
		{
			m_spConsole->setMessage(MSG_ERROR, "qInput", "initKeyboard : Keyboard device cannot be created.");
			return false;
		}

		if (FAILED(m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		{
			m_spConsole->setMessage(MSG_ERROR, "qInput", "initKeyboard : Keyboard data format cannot be set.");
			return false;
		}

		if (FAILED(m_pDIKeyboard->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		{
			m_spConsole->setMessage(MSG_ERROR, "qInput", "initKeyboard : Keyboard device coop level cannot be set.");
			return false;
		}

		if (FAILED(m_pDIKeyboard->Acquire()))
		{
			m_spConsole->setMessage(MSG_ERROR, "qInput", "initKeyboard : Keyboard device cannot be acquired.");
			return false;
		}
		
		return true;
	}


/********************************* qInput::readKeyboard **********************************
description:
   Reads the current keyboard state.

value:
   Returns true if keyboard has been read successfully, otherwise returns false (in case
   of lost device).

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 17:24:08
creator: qDot
*****************************************************************************************/

	bool qInput::readKeyboard()
	{
		if (FAILED(m_pDIKeyboard->GetDeviceState(sizeof(UCHAR[256]), (LPVOID)m_pKeyState)))
		{
			return false;
		}
		return true;
	}

/*********************************** qInput::keyDown *************************************
description:
   Returns true if the key requested is currently in a "down" state.

@key[in]:
   DIK constant pertaining to the key needed.

value:
   Returns true if the key requested is down.

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 17:25:24
creator: qDot
*****************************************************************************************/

	bool qInput::keyDown(DWORD key)
	{
		return ((m_pKeyState[key] & 0x80) ? true : false);
	}

/********************************** qInput::initMouse ************************************
description:
   Initializes the Mouse Device

value:
   Returns true if the mouse is successfully initialized.

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 17:26:23
creator: qDot
*****************************************************************************************/

	bool qInput::initMouse()
	{

		if (FAILED(m_pDI->CreateDevice(GUID_SysMouse, &m_pDIMouse, NULL)))
		{
			m_spConsole->setMessage(MSG_ERROR, "qInput", "initMouse : Mouse device cannot be created.");
			return false;
		}
		if (FAILED(m_pDIMouse->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		{
			m_spConsole->setMessage(MSG_ERROR, "qInput", "initMouse : Mouse device coop level cannot be set.");
			return false;
		}
		if (FAILED(m_pDIMouse->SetDataFormat(&c_dfDIMouse)))
		{
			m_spConsole->setMessage(MSG_ERROR, "qInput", "initMouse : Data format cannot be set.");
			return false;
		}
		if (FAILED(m_pDIMouse->Acquire()))
		{
			m_spConsole->setMessage(MSG_ERROR, "qInput", "initMouse : Mouse device cannot be aquired.");
			return false;
		}

		return true;
	}

/********************************** qInput::readMouse ************************************
description:
   Reads the current state of the mouse, and updates the buffered input variables.

value:
   Returns true if the device is read successfully, otherwise returns false (possibly from
   lost device).

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 17:28:58
creator: qDot
*****************************************************************************************/

	bool qInput::readMouse()
	{
		if(FAILED(m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_DIMouseState)))
		{
			m_spConsole->setMessage(MSG_ERROR, "qInput", "readMouse : Cannot read mouse device.");
			return false;
		}
		m_lMouseX += m_DIMouseState.lX;
		m_lMouseY += m_DIMouseState.lY;
		if(m_lMouseX < 0) m_lMouseX = 0;
		if (m_lMouseY < 0) m_lMouseY = 0;
		if (m_lMouseX > m_iWindowWidth) m_lMouseX = (long)m_iWindowWidth;
		if (m_lMouseY > m_iWindowHeight) m_lMouseY = (long)m_iWindowHeight;
		return true;
	}

/******************************** qInput::mousePosition **********************************
description:
   Returns the mouse position into two pointers passed to it.

@lX[in]:
   Pointer to a long variable to store the x coord to.
@lY[in]:
   Pointer to a long variable to store the y coord to.

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 17:29:42
creator: qDot
*****************************************************************************************/

	void qInput::getMousePosition(long* lX, long* lY)
	{
		*lX = m_lMouseX;
		*lY = m_lMouseY;
	}

/******************************* qInput::mouseButtonDown *********************************
description:
   Checks the state of mouse buttons.

@iButton[in]:
   Number of the button to check. 
   0 - Left
   1 - Right

value:
   Returns true if the button requested is down, otherwise returns false.

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 17:30:52
creator: qDot
*****************************************************************************************/

	bool qInput::getMouseButtonDown(int iButton)
	{
		return (m_DIMouseState.rgbButtons[iButton] & 0x80) ? true : false;
	}


/*********************************** qInput::shutdown ************************************
description:
   Releases all COM objects that have been initialized.

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 17:31:10
creator: qDot
*****************************************************************************************/

	void qInput::shutdown()
	{

		if(m_pDIKeyboard)
		{
			m_pDIKeyboard->Unacquire();
			m_pDIKeyboard->Release();
		}
		if(m_pDIMouse)
		{
			m_pDIMouse->Unacquire();
			m_pDIMouse->Release();
		}
		if(m_pDI)
		{
			m_pDI->Release();
		}
		
		m_spConsole->setMessage(MSG_SHUTDOWN, "qInput", "Shutdown() : DI8 Shutdown successful");
		m_spConsole.reset();
	}

/*********************************** qInput::~qInput *************************************
description:
   Destructor. Calls the shutdown function.

access: public, state: virtual
------------------------------------------------------------------------------------------
created: 30.10.02 - 17:32:51
creator: qDot
*****************************************************************************************/

	qInput::~qInput(void)
	{
		shutdown();
	}

	void qInput::getValuesFromConfigManager()
	{
		//Values to get from Console:
		//WindowWidth - Window Width
		//WindowHeight - Window Height

		m_iWindowWidth = boost::lexical_cast<int>(m_spCM->getValue("WindowWidth"));
		m_iWindowHeight = boost::lexical_cast<int>(m_spCM->getValue("WindowHeight"));
	}

}
