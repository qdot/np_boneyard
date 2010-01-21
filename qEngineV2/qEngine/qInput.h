/************************************************************************
Class Name: qInput
Date: 10/30/02
Purpose: Handles DirectInput objects and functions, and catches device
states.
************************************************************************/

/************************************************************************/
/* Includes                                                             */
/************************************************************************/

#pragma once
#ifndef Q_INPUT
#define Q_INPUT

#define Q_KEYBOARD 0x00000001
#define Q_MOUSE 0x00000010
#define Q_JOYSTICK 0x00000100

#include <qEngine/qCommonInclude.h>
#include <qEngine/qConsole.h>
#include <qEngine/qConfigManager.h>

/************************************************************************/
/* Class Declarations                                                   */
/************************************************************************/

namespace qEngine
{

	class qInput
	{
	public:
		qInput(void);
		
		//Manager functions for input configurations
		void addInputConfig();
		void removeInputConfig();
		void setInputConfig();

		bool init(HWND hWnd, boost::shared_ptr<qConsole> spConsole, boost::shared_ptr<qConfigManager>, DWORD dwDevices);
		void shutdown(void);		
		
		bool initKeyboard();
		bool readKeyboard();
		bool keyDown(DWORD key);

		bool initMouse();
		bool readMouse();
        void getMousePosition(long* lX, long* lY);
		bool getMouseButtonDown(int iButton);
		
		void getValuesFromConfigManager();

		void updateDevices();

		~qInput(void);
	
	private:
		LPDIRECTINPUT8 m_pDI;
		LPDIRECTINPUTDEVICE8 m_pDIKeyboard;
		LPDIRECTINPUTDEVICE8 m_pDIMouse;

		unsigned char m_pKeyState[256];

		HWND m_hWnd;

		DIMOUSESTATE m_DIMouseState;
		long m_lMouseX;
		long m_lMouseY;

		int m_iWindowWidth;
		int m_iWindowHeight;

		boost::shared_ptr<qConfigManager> m_spCM;
		boost::shared_ptr<qConsole> m_spConsole;
	};
}

#endif