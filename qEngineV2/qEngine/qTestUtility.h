#pragma once
#ifndef Q_TU
#define Q_TU

#include <qEngine/qCommonInclude.h>
#include <qEngine/qConsole.h>
#include <qEngine/qInput.h>
#include <qEngine/qStateManager.h>
#include <qEngine/qProcessManager.h>
#include <qEngine/qDirect3D.h>

namespace qEngine
{
	class qTestUtility
	{
	public:
		qTestUtility(void);
		qTestUtility(boost::shared_ptr< qConsole > spConsole);
		~qTestUtility(void);
		void displayConsoleMessages(int iWindowHeight, float FPS);
	private:
		boost::shared_ptr< qConsole > m_spConsole;
		LPD3DXFONT lpFont;
		LOGFONT lf;

	};
}

#endif