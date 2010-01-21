#include "qtestutility.h"

namespace qEngine
{
	qTestUtility::qTestUtility(void)
	{
	}

	qTestUtility::qTestUtility(boost::shared_ptr< qConsole > spConsole) : m_spConsole(spConsole)
	{
		ZeroMemory(&lf, sizeof(LOGFONT));
		strcpy(lf.lfFaceName, "Times New Roman");
		lf.lfHeight = -12;
		if(FAILED(D3DXCreateFontIndirect(g_lpD3DDevice, &lf, &lpFont)))
		{
			m_spConsole->setMessage(MSG_ERROR, "qTestUtility", "init : Cannot create font for test utility");
		}
		m_spConsole->setMessage(MSG_INIT, "qTestUtility", "constructor : Test utility initialized");
	}

	void qTestUtility::displayConsoleMessages(int iWindowHeight, float FPS)
	{
		RECT rect = {0, 0, iWindowHeight, iWindowHeight};
		int iNumLines = (int) (iWindowHeight / 12);
		vector<string> vMsgBuffer;
		m_spConsole->getMessages(iNumLines, &vMsgBuffer);
		string title = "qEngine v1.0 FPS:" + boost::lexical_cast<string>(FPS);
		if(SUCCEEDED(g_lpD3DDevice->BeginScene()))
		{
			lpFont->DrawText(title.c_str(), -1, &rect, 0, D3DCOLOR_RGBA(255,255,255,255));
			rect.top = 12;
			for(UINT i = 1; i < vMsgBuffer.size(); ++i)
			{
				lpFont->DrawText(vMsgBuffer[i - 1].c_str(), -1, &rect, 0, D3DCOLOR_RGBA(255,255,255,255));
				rect.top = ((i) + 1)*12;
				rect.bottom = iWindowHeight;
				rect.left = 0;
				rect.right = iWindowHeight;
			}
		}
		g_lpD3DDevice->EndScene();
	}


	qTestUtility::~qTestUtility(void)
	{
		if(lpFont)
		{
			lpFont->Release();
		}
	}
}