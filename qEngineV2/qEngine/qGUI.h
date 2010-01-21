/********************************************************************
	created:	2002/11/26
	created:	26:11:2002   2:18
	filename: 	c:\documents and settings\qdot\my documents\gameproject\qenginev2\qengine\qgui.h
	file path:	c:\documents and settings\qdot\my documents\gameproject\qenginev2\qengine
	file base:	qgui
	file ext:	h
	author:		qDot <qDot@numberporn.com>
	
	purpose:	The GUI class takes a copy of the Renderer so it can 
				handle all of its rendering itself. Since fonts are 
				just textures with extra operations on them, it 
				handles those extra operations and leaves the
				texture work to the renderer. This class will also
				take care of rendering any HUD type 2d overlay 
				displays for games, as well as the console,
				custom cursors, and anything else that's an 
				overlay..
*********************************************************************/

#pragma once

#include <qEngine/qCommonInclude.h>
#include <qEngine/qRenderer.h>
#include <qEngine/qVertex.h>
#include <qEngine/DynamicVB.h>
#include <qEngine/qConfigManager.h>
#include <qEngine/qPosition.h>

#define MAX_STRING_COUNT 100


namespace qEngine
{
	class qGUI
	{
	public:

		qGUI(void);
		qGUI(boost::shared_ptr<qRenderer> spRenderer);
		~qGUI(void);
		
		/* Font resource management */

		bool loadFont(string strName);
		inline void setFont(string strName) { m_strCurrentFont = strName; }
		bool unloadFont(string strName);

		/* Font/Text rendering functions */

		void drawText(string strText, float iX, float iY, DWORD dwColor, float fScale = 1.0);

		/* Cursor resource management */

		bool loadCursor(string strName);
		inline void setCursor(string strName) { m_strCurrentCursor = strName; }
		bool unloadCursor(string strName);
		void drawCursor();

		/* Cursor rendering/state functions */

		inline void showCursor(bool bShow) { m_bShowCursor = bShow; }
		inline void setCursorPos(int iX, int iY) { m_iMouseX = iX, m_iMouseY = iY; }

		/* Console functions */

		inline void showConsole(bool bShow) { m_bShowConsole = bShow; }
		void renderConsole();

		/* Text rendering functions */
		void addText(string strText, float iX, float iY, DWORD dwColor, float fScale);
		void addText(string strText, int iX, int iY, DWORD dwColor, float fScale);
		void clearText();		

		/* Misc functions */

		bool init(boost::shared_ptr<qConfigManager> spCM);
		bool renderGUI();
		void convScreenToWorld(D3DXVECTOR3 &vOut, float fX, float fY);
		void convWorldToScreen(D3DXVECTOR3 &vOut, float fX, float fY);

	private:
		void renderText();
		void renderCursor();

		/* GUI Initialization functions */

		void getValuesFromCM(boost::shared_ptr<qConfigManager> spCM);
		float m_fScreenWidth;
		float m_fScreenHeight;

		qEngine::qPosition m_qpCursor;

		/* Vertex we are currently on for the renderer */

		int m_iStartVertex;
		
		/* Member variables for mouse position tracking */
		int m_iMouseX;
		int m_iMouseY;

		/* Boolean variables for display status of 2D Engine Objects */
		bool m_bShowConsole;
		bool m_bShowCursor;

		/* String variables to keep track of the current textures */
		string m_strCurrentCursor;
		string m_strCurrentFont;

		/* Shared renderer for setting/retrieving VB functions and textures */
		boost::shared_ptr< qRenderer > m_spRenderer;

		/* Text drawing specific variables and containers */

		struct qTextStruct
		{
			D3DXVECTOR3 vTextPos;
			string strFont;
			string strText;
			DWORD dwColor;
		};

		vector<qTextStruct> m_vstrTextBuffer;

		DWORD oldState;
		/* Dynamic Vertex Buffer */
		boost::shared_ptr< DynamicVB< TEXPROC_VERTEX > > m_vbText;
		boost::shared_ptr< DynamicVB< TEX_VERTEX > > m_spVB;
	};
}