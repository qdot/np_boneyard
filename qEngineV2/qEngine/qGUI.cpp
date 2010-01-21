#include "qgui.h"

//TODO: Add resource vectors to check for addition and destruction
//TODO: Add scene graph and viewport class SOON.

namespace qEngine
{

	qGUI::qGUI(boost::shared_ptr<qRenderer> spRenderer) : m_bShowCursor(false), m_bShowConsole(false), m_iMouseX(0), m_iMouseY(0), m_spRenderer(spRenderer), m_iStartVertex(0)
	{
		m_spVB.reset(new DynamicVB<TEX_VERTEX>(g_lpD3DDevice, D3DFVF_TexturedVertex, 5000));
		m_vbText.reset(new DynamicVB<TEXPROC_VERTEX>(g_lpD3DDevice, D3DFVF_TexturedProcessedVertex, 5000));
		m_vstrTextBuffer.reserve(MAX_STRING_COUNT);

		drawCursor();
	}

	bool qGUI::loadCursor(string strName)
	{
		if(m_spRenderer->loadTexture(strName))
		{
			if(m_strCurrentCursor == "") setCursor(strName);
			return true;
		}
		return false;
	}

	bool qGUI::unloadCursor(string strName)
	{
		if(strName == m_strCurrentCursor)
		{
			showCursor(false);
		}
		return m_spRenderer->unloadTexture(strName);
	}

	void qGUI::drawCursor()
	{
			TEX_VERTEX* pVB = m_spVB->Lock(0, (UINT&)m_iStartVertex);
			//Draws the cursor (16x16) at 0,0
			pVB[m_iStartVertex].pos = D3DXVECTOR3((0.0f), (0.0f), 0.0);
			pVB[m_iStartVertex].color = D3DCOLOR_RGBA(255,255,255,255);
			pVB[m_iStartVertex].tu = 0.0;
			pVB[m_iStartVertex].tv = 0.0;
			pVB[m_iStartVertex+1].pos = D3DXVECTOR3((0.0f) + 16, (0.0f), 0.0);
			pVB[m_iStartVertex+1].color = D3DCOLOR_RGBA(255,255,255,255);
			pVB[m_iStartVertex+1].tu = 1.0;
			pVB[m_iStartVertex+1].tv = 0.0;
			pVB[m_iStartVertex+2].pos = D3DXVECTOR3((0.0f), (0.0f) - 16, 0.0);
			pVB[m_iStartVertex+2].color = D3DCOLOR_RGBA(255,255,255,255);
			pVB[m_iStartVertex+2].tu = 0.0;
			pVB[m_iStartVertex+2].tv = 1.0;
			pVB[m_iStartVertex+3].pos = D3DXVECTOR3((0.0f) + 16, (0.0f) - 16, 0.0);
			pVB[m_iStartVertex+3].color = D3DCOLOR_RGBA(255,255,255,255);
			pVB[m_iStartVertex+3].tu = 1.0;
			pVB[m_iStartVertex+3].tv = 1.0;
			
			m_spVB->Unlock();
			pVB = NULL;
			m_iStartVertex += 4;
	}

	bool qGUI::loadFont(string strName)
	{
		if(m_spRenderer->loadTexture(strName))
		{
			if(m_strCurrentFont == "") setFont(strName);
			return true;
		}
		return false;
	}

	bool qGUI::unloadFont(string strName)
	{
		m_strCurrentFont = "";
		return m_spRenderer->unloadTexture(strName);
	}

	bool qGUI::renderGUI()
	{
		m_iStartVertex = 0;
		//m_spVB->FlushAtFrameStart();
		m_spRenderer->setRenderState( D3DRS_ALPHABLENDENABLE, true );

		//If we're showing the cursor and the texture requested for the current cursor is set,
		//render the cursor
		if(m_bShowCursor && m_spRenderer->setTexture(m_strCurrentCursor, 0))
		{	
			renderCursor();
		}
		renderText();
		return true;
	}
	
	void qGUI::renderCursor()
	{
		D3DXVECTOR3 vWorld;
		convScreenToWorld(vWorld, (float)m_iMouseX, (float)m_iMouseY);
		m_qpCursor.move(vWorld.x, vWorld.y, 0.0f);
		m_qpCursor.updateMatrix();
		m_qpCursor.setView();

		g_lpD3DDevice->SetVertexShader(D3DFVF_TexturedVertex);
		g_lpD3DDevice->SetStreamSource(0, m_spVB->GetInterface(), sizeof(TEX_VERTEX));
		g_lpD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	qGUI::~qGUI(void)
	{
		m_vbText.reset();	
		m_spVB.reset();
	}

	bool qGUI::init(boost::shared_ptr<qConfigManager> spCM)
	{
		getValuesFromCM(spCM);
		return true;
	}

	void qGUI::convScreenToWorld(D3DXVECTOR3 &vOut, float fX, float fY)
	{
		D3DXMATRIX matProj;
		g_lpD3DDevice->GetTransform( D3DTS_PROJECTION, &matProj );
		vOut.x = ( ( ( 2.0f * fX ) /  m_fScreenWidth) - 1 ) / matProj._11;
		vOut.y = -( ( ( 2.0f * fY ) /  m_fScreenHeight) - 1 ) / matProj._22;
		vOut.z = 1.0f;
	}

	void qGUI::getValuesFromCM(boost::shared_ptr<qConfigManager> spCM)
	{
		//if we've gotten this far, these values are guarenteed to exist (otherwise window
		//creation would've failed.
		m_fScreenWidth = boost::lexical_cast<float>(spCM->getValue("WindowWidth"));
		m_fScreenHeight = boost::lexical_cast<float>(spCM->getValue("WindowHeight"));
	}

	void qGUI::addText(string strText, int iX, int iY, DWORD dwColor, float fScale)
	{
		addText(strText, (float)iX, (float)iY, dwColor, fScale);
	}

	void qGUI::addText(string strText, float fX, float fY, DWORD dwColor, float fScale)
	{
		qTextStruct qtTmp;
		qtTmp.strFont = m_strCurrentFont;
		qtTmp.strText = strText;
		qtTmp.vTextPos = D3DXVECTOR3(fX, fY, 0.0f);
		qtTmp.dwColor = dwColor;
		m_vstrTextBuffer.push_back(qtTmp);
	}

	void qGUI::clearText()
	{
		m_vstrTextBuffer.clear();
	}

	void qGUI::renderText()
	{
		UINT iNumChars = 0; //Number of characters to draw
		UINT iCharPos = 0;
		UINT iChr = 0;
		UINT iStart = 0;
		float fScreenX1= 0.0f;
		float fScreenY1= 0.0f;
		float fScreenX2= 0.0f;
		float fScreenY2= 0.0f;
		float fBMX1 = 0.0f;
		float fBMX2 = 0.0f;
		float fBMY1 = 0.0f;
		float fBMY2 = 0.0f;
		TEXPROC_VERTEX* pVB = m_vbText->Lock(0, iStart);
		for(UINT i = 0; i < (int)m_vstrTextBuffer.size(); ++i)
		{
			qTextStruct qtCurString = m_vstrTextBuffer[i];
			if(!(qtCurString.strFont == "") && !(qtCurString.strText == ""))
			{
				fScreenX1 = qtCurString.vTextPos.x;
				fScreenY1 = qtCurString.vTextPos.y;

				// cycle through all the characters in the string
				for (iCharPos = iNumChars; iCharPos < (int)qtCurString.strText.length() + iNumChars; ++iCharPos)
				{
					// isolate the character we are interested in
					iChr = qtCurString.strText[iCharPos - iNumChars];

					// screen space dimensions for this character
					/*
					screenX2= screenX1 + (float)charWidths[chr] * scale;
					screenY2= screenY1 + (float)cellHeight * scale;
					*/
					//TODO: Set fonts to variable width instead of making them all fixed.
					fScreenX2 = fScreenX1 + 32.0f;
					fScreenY2 = fScreenY1 + 32.0f;

					// which part of the bitmap do we want for this character, scaled from 0 to 1
					/*
					float bmy1= float( (cellHeight)*(chr/16) ) / float(bmHeight);
					float bmx1= float( (cellWidth)*(chr%16) ) / float(bmWidth); 
					float bmy2= bmy1 + (float(cellHeight)) / (float(bmHeight));
					float bmx2= bmx1 + (float(charWidths[chr])) / (float(bmWidth));
					*/
					fBMY1 = (float)(32.0f*(iChr/16)) / (float)(512.0f);
					fBMX1 = (float)(32.0f*(iChr%16)) / (float)(512.0f);
					fBMY2 = fBMY1 + (32.0f) / (512.0f);
					fBMX2 = fBMX1 + (32.0f) / (512.0f);

					// set the vertex buffer data for each character.
					// Each character will be made up of 2 triangles, so we will have 6 verts per character
					pVB[iCharPos*6+0].pos = D3DXVECTOR3(fScreenX1-0.5f, fScreenY2-0.5f, 0.0f);	pVB[iCharPos*6+0].rhw= 1.0f; pVB[iCharPos*6+0].color= qtCurString.dwColor; 
					pVB[iCharPos*6+1].pos = D3DXVECTOR3(fScreenX1-0.5f, fScreenY1-0.5f, 0.0f);	pVB[iCharPos*6+1].rhw= 1.0f; pVB[iCharPos*6+1].color= qtCurString.dwColor;	
					pVB[iCharPos*6+2].pos = D3DXVECTOR3(fScreenX2-0.5f, fScreenY2-0.5f, 0.0f);	pVB[iCharPos*6+2].rhw= 1.0f; pVB[iCharPos*6+2].color= qtCurString.dwColor; 
					pVB[iCharPos*6+3].pos = D3DXVECTOR3(fScreenX1-0.5f, fScreenY1-0.5f, 0.0f);	pVB[iCharPos*6+3].rhw= 1.0f; pVB[iCharPos*6+3].color= qtCurString.dwColor;	
					pVB[iCharPos*6+4].pos = D3DXVECTOR3(fScreenX2-0.5f, fScreenY2-0.5f, 0.0f);	pVB[iCharPos*6+4].rhw= 1.0f; pVB[iCharPos*6+4].color= qtCurString.dwColor; 
					pVB[iCharPos*6+5].pos = D3DXVECTOR3(fScreenX2-0.5f, fScreenY1-0.5f, 0.0f);	pVB[iCharPos*6+5].rhw= 1.0f; pVB[iCharPos*6+5].color= qtCurString.dwColor;	
					
					pVB[iCharPos*6+0].tu= fBMX1; pVB[iCharPos*6+0].tv= fBMY2;
					pVB[iCharPos*6+1].tu= fBMX1; pVB[iCharPos*6+1].tv= fBMY1;
					pVB[iCharPos*6+2].tu= fBMX2; pVB[iCharPos*6+2].tv= fBMY2;
					pVB[iCharPos*6+3].tu= fBMX1; pVB[iCharPos*6+3].tv= fBMY1;
					pVB[iCharPos*6+4].tu= fBMX2; pVB[iCharPos*6+4].tv= fBMY2;
					pVB[iCharPos*6+5].tu= fBMX2; pVB[iCharPos*6+5].tv= fBMY1;
					
					// increase the starting position of the next character
					fScreenX1 += 32.0f;
					//screenX1+= charWidths[chr] * scale;
				}
				iNumChars += (UINT) qtCurString.strText.length();
			}
		}
		m_vbText->Unlock();
		
		m_spRenderer->setTexture(m_strCurrentFont, 0);
		g_lpD3DDevice->SetVertexShader(D3DFVF_TexturedProcessedVertex);
		g_lpD3DDevice->SetStreamSource(0, m_vbText->GetInterface(), sizeof(TEXPROC_VERTEX));
		g_lpD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, iNumChars*2);
	}
}