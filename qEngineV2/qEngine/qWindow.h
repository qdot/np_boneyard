#pragma once

#include <qEngine/qCommonInclude.h>
#include <qEngine/qConsole.h>
#include <qEngine/qConfigManager.h>

namespace qEngine 
{
	class qWindow
	{
		public:
			enum enmWindowStatus { WIN_NOT_CREATED, WIN_ALIVE, WIN_DEAD, WIN_ERROR }; //enumeration of window status levels


			qWindow() {}; //Constructor which takes a pointer to a console
			qWindow(boost::shared_ptr< qConsole > spConsole);
			virtual ~qWindow(void); //Destructor

			//Creates and registers the window.
			bool create();

			virtual bool init();
			virtual void shutdown();

			//Status Handlers
			void setDead() {m_eStatus = WIN_DEAD;}
			bool isAlive() {return m_eStatus == WIN_ALIVE;}
			bool hasError() {return m_eStatus == WIN_ERROR;}

			HWND gethWnd() { return m_hWnd;	}

			//Event Managers
			void handleMessages(); 
			void addHandler(UINT message, UINT handler); 
			void removeHandler(UINT message);

			//Basic Window Display functions
			void show();
			void hide();

		protected:
			
			virtual void getValuesFromConfigManager(boost::shared_ptr< qConfigManager > spCM);
			void beginFullScreen();
			void endFullScreen();


			//Variables for window setup and status
			enmWindowStatus m_eStatus;

			//Console
			boost::shared_ptr<qConsole> m_spConsole;

			//Message Router
			static LRESULT CALLBACK msgRouter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

			//Window Messages to be handled at this level

			//WM_CLOSE
			static void onClose(qWindow &wnd, HWND hWnd, WPARAM wParam, LPARAM lParam);
			//WM_DESTROY
			static void onDestroy(qWindow &wnd, HWND hWnd, WPARAM wParam, LPARAM lParam);
			//WM_PAINT
			static void onPaint(qWindow &wnd, HWND hWnd, WPARAM wParam, LPARAM lParam);

			//void getValuesFromConsole();

			//Window properties
			HDC	m_hDC;		// device context
			HGLRC m_hGLRC;	// rendering context
			HWND m_hWnd;	// window handle
			map<UINT, UINT> m_MsgHandlers;	//Window Message Handler Container

			int m_iWindowWidth;
			int m_iWindowHeight;
			int m_iWindowBPP;
			int m_iWindowRefreshRate;
			bool m_bWindowedMode;
			string m_strWindowTitle;
	};
}