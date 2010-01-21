#include "qtexture.h"

namespace qEngine
{

	qTexture::qTexture(void)
	{
		m_pD3DTexture = NULL;
	}

	bool qTexture::loadResource(string strFilename)
	{
		D3DFORMAT format= D3DFMT_A8R8G8B8;
		return SUCCEEDED(D3DXCreateTextureFromFileA(g_lpD3DDevice, strFilename.c_str(), &m_pD3DTexture));
	}


	LPDIRECT3DTEXTURE8 qTexture::getResource()
	{
		if(m_pD3DTexture != NULL)
		{
			return m_pD3DTexture;
		}
		return NULL;
	}

	bool qTexture::unloadResource()
	{
		if(m_pD3DTexture != NULL)
		{
			m_pD3DTexture->Release();
			return true;
		}
		return false;
	}

	qTexture::~qTexture(void)
	{
		unloadResource();
	}
}