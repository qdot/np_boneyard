#include "qviewport.h"

namespace qEngine
{

	qViewport::qViewport(void) : m_fScreenWidth(0.0f), m_fScreenHeight(0.0f), m_fOrthoClipNear(0.0f), 
		m_fOrthoClipFar(0.0f), m_fPOV(0.0f), m_fProjClipNear(0.0f), m_fProjClipFar(0.0f)
	{
	}

	qViewport::~qViewport(void)
	{
	}

	void qViewport::setScreenDimensions(float fWidth, float fHeight)
	{
		m_fScreenHeight = fHeight;
		m_fScreenWidth = fWidth;
	}

	void qViewport::setOrthoViewport(void)
	{
		setOrthoViewport(m_fScreenWidth, m_fScreenHeight, m_fOrthoClipNear, m_fOrthoClipFar);
	}

	void qViewport::setOrthoViewport(float fNear, float fFar)
	{
		setOrthoViewport(m_fScreenWidth, m_fScreenHeight, fNear, fFar);
	}

	void qViewport::setOrthoViewport(float fWidth, float fHeight, float fNear, float fFar)
	{
		m_fScreenHeight = fHeight;
		m_fScreenWidth = fWidth;
		m_fOrthoClipNear = fNear;
		m_fOrthoClipFar = fFar;

		D3DXMATRIX Ortho2D;
		D3DXMATRIX Identity;
		
		D3DXMatrixOrthoLH(&Ortho2D, fWidth, fHeight, fNear, fFar);
		D3DXMatrixIdentity(&Identity);

		g_lpD3DDevice->SetTransform(D3DTS_PROJECTION, &Ortho2D);
		g_lpD3DDevice->SetTransform(D3DTS_WORLD, &Identity);
		g_lpD3DDevice->SetTransform(D3DTS_VIEW, &Identity);
	}

	
	void qViewport::setProjViewport(void)
	{
		setProjViewport(m_fScreenWidth, m_fScreenHeight, m_fProjClipNear, m_fProjClipFar);
	}
	
	void qViewport::setProjViewport(float fNear, float fFar)
	{
		setProjViewport(m_fScreenWidth, m_fScreenHeight, fNear, fFar);
	}

	void qViewport::setProjViewport(float fWidth, float fHeight, float fNear, float fFar)
	{
		D3DXMATRIX matProj;
		D3DXMatrixPerspectiveFovLH(&matProj, m_fPOV, m_fScreenWidth/m_fScreenHeight, 1.0f, 1000.0f );
		g_lpD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );
	}

	void qViewport::getValuesFromCM(boost::shared_ptr<qConfigManager> spCM)
	{
		//if we've gotten this far, these values are guarenteed to exist (otherwise window
		//creation would've failed.
		m_fScreenWidth = boost::lexical_cast<float>(spCM->getValue("WindowWidth"));
		m_fScreenHeight = boost::lexical_cast<float>(spCM->getValue("WindowHeight"));

		//These values should be set by the client program on first run. However, if they
		//are not, the program will do very bad things. Therefore we set some predefined
		//constants just in case.
		if(!spCM->hasValue("ProjectionPOV")) m_fPOV = D3DX_PI/4;
		else m_fPOV = boost::lexical_cast<float>(spCM->getValue("ProjectionPOV"));
		
		if(!spCM->hasValue("ProjectionClipNear")) m_fProjClipNear = 1.0f;
		else m_fProjClipNear = boost::lexical_cast<float>(spCM->getValue("ProjectionClipNear"));

		if(!spCM->hasValue("ProjectionClipFar")) m_fProjClipNear = 1000.0f;
		else m_fProjClipFar = boost::lexical_cast<float>(spCM->getValue("ProjectionClipFar"));

		if(!spCM->hasValue("OrthoClipNear")) m_fOrthoClipNear = 0.0f;
		else m_fOrthoClipNear = boost::lexical_cast<float>(spCM->getValue("OrthoClipNear"));

		if(!spCM->hasValue("OrthoClipFar")) m_fOrthoClipFar = 1.0f;
		else m_fOrthoClipFar = boost::lexical_cast<float>(spCM->getValue("OrthoClipFar"));
	}

	void qViewport::setValuesInCM(boost::shared_ptr<qConfigManager> spCM)
	{
		spCM->setValue("ProjectionPOV", boost::lexical_cast<string>(m_fPOV));
		spCM->setValue("ProjectionClipNear", boost::lexical_cast<string>(m_fProjClipNear));
		spCM->setValue("ProjectionClipFar", boost::lexical_cast<string>(m_fProjClipFar));
		spCM->setValue("ProjectionOrthoNear", boost::lexical_cast<string>(m_fOrthoClipNear));
		spCM->setValue("ProjectionOrthoFar", boost::lexical_cast<string>(m_fOrthoClipFar));
	}
}