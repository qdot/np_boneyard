#include "qrenderer.h"

namespace qEngine
{

/************************************************************************/
/* Construction/Destruction Functions                                   */
/************************************************************************/
	
	qRenderer::qRenderer(void)
	{	
		
	}

	qRenderer::qRenderer(boost::shared_ptr<qConsole> spConsole) : m_spConsole(spConsole)
	{
		m_rmTextureMgr.reset(new qResourceManager<qTexture>(m_spConsole, "Texture Manager"));
		m_rmXFileMgr.reset(new qResourceManager<qXFile>(m_spConsole,"XFile Manager"));
	}

	qRenderer::~qRenderer(void)
	{
		m_rmXFileMgr.reset();
		m_rmTextureMgr.reset();
	}


/************************************************************************/
/* Basic Rendering Functions                                            */
/************************************************************************/

	HRESULT qRenderer::clearScreen()
	{
		return g_lpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
	}

	HRESULT qRenderer::presentBuffer()
	{
		return g_lpD3DDevice->Present( NULL, NULL, NULL, NULL );
	}

/************************************************************************/
/* Texturing Functions                                                  */
/************************************************************************/
 
	bool qRenderer::loadTexture(string strName)
	{
		if(m_rmTextureMgr->getResource(strName) != NULL)
		{
			m_spConsole->setMessage(MSG_COMMON, "qRenderer", "loadTexture : Texture " + strName + " loaded successfully.");
			return true;
		}
		m_spConsole->setMessage(MSG_ERROR, "qRenderer", "loadTexture : Texture " + strName + " not loaded.");
		return false;
	}

	bool qRenderer::setTexture(string strName, int iStage)
	{
		qTexture* _spTempTex = m_rmTextureMgr->getResource(strName);
		if(_spTempTex != NULL)
		{
			g_lpD3DDevice->SetTexture(iStage, _spTempTex->getResource());
			return true;
		}
		return false;
	}

	bool qRenderer::unloadTexture(string strName)
	{
		return m_rmTextureMgr->deleteResource(strName);
	}

/************************************************************************/
/* Mesh Functions                                                       */
/************************************************************************/

	bool qRenderer::drawMesh(string strName)
	{

		if(m_rmXFileMgr->hasResource(strName))
		{
			qXFile* _spTempMesh = m_rmXFileMgr->getResource(strName);
			if(_spTempMesh != NULL)
			{
				qXFile::sMeshInfo _sMesh = _spTempMesh->getResource();
				if(SUCCEEDED(g_lpD3DDevice->BeginScene()))
				{
					int i = 0;
					for(UINT i=0; i < _sMesh.dwNumMaterials; ++i )
					{
						setTexture(_sMesh.pstrTextureArray[i], 0);
						g_lpD3DDevice->SetMaterial(&_sMesh.pD3DMaterialArray[i]);
						_sMesh.lpD3DXMesh->DrawSubset(i);
					}
					g_lpD3DDevice->EndScene();
				}
				return true;
			}
		}
		return false;
	}

	bool qRenderer::loadMesh(string strName)
	{
		if (m_rmXFileMgr->getResource(strName) != NULL)
		{
			m_spConsole->setMessage(MSG_COMMON, "qRenderer", "loadMesh : Mesh " + strName + " loaded successfully.");
			return true;
		}
		m_spConsole->setMessage(MSG_COMMON, "qRenderer", "loadMesh : Mesh " + strName + " not loaded.");
		return false;
	}

	bool qRenderer::unloadMesh(string strName)
	{
		if(m_rmXFileMgr->hasResource(strName))
		{
			return m_rmXFileMgr->deleteResource(strName);
		}
		return false;
	}

	/************************************************************************/
/* Misc Functions                                                       */
/************************************************************************/

	bool qRenderer::setRenderState(const D3DRENDERSTATETYPE& state, const DWORD& value)
	{
		return SUCCEEDED(g_lpD3DDevice->SetRenderState( state, value ));
	}

	bool qRenderer::setTextureStageState(const DWORD& stage, const D3DTEXTURESTAGESTATETYPE& state, const DWORD& value )
	{
		return SUCCEEDED(g_lpD3DDevice->SetTextureStageState(stage, state, value));
	}

	void qRenderer::setCamera()
	{

		static UINT i = 0;
		// For our world matrix, we will just leave it as the identity
		D3DXMATRIX matWorld;
		D3DXMatrixRotationY( &matWorld, ++i/100.0f );
		g_lpD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

		// Set up our view matrix. A view matrix can be defined given an eye point,
		// a point to lookat, and a direction for which way is up. Here, we set the
		// eye five units back along the z-axis and up three units, look at the 
		// origin, and define "up" to be in the y-direction.
		D3DXMATRIX matView;
		D3DXMatrixLookAtLH( &matView, &D3DXVECTOR3( 0.0f, 0.0f,-5.0f ), 
									&D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), 
									&D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
		g_lpD3DDevice->SetTransform( D3DTS_VIEW, &matView );

		// For the projection matrix, we set up a perspective transform (which
		// transforms geometry from 3D view space to 2D viewport space, with
		// a perspective divide making objects smaller in the distance). To build
		// a perpsective transform, we need the field of view (1/4 pi is common),
		// the aspect ratio, and the near and far clipping planes (which define at
		// what distances geometry should be no longer be rendered).
		D3DXMATRIX matProj;
		D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1280.0/1024.0, 1.0f, 1000.0f );
		g_lpD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );
	}
}