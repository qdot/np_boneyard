#pragma once
#ifndef Q_TEXTURE
#define Q_TEXTURE

#include <qEngine/qCommonInclude.h>
#include <qEngine/qResourceManager.h>
#include <qEngine/qResource.h>

namespace qEngine
{

	class qTexture : public qResource
	{
	public:
		qTexture(void);
		//qTexture(const qTexture& _Copy);
		~qTexture(void);
		bool loadResource(string strFilename);
		LPDIRECT3DTEXTURE8 getResource();
		bool unloadResource();
	private:
		LPDIRECT3DTEXTURE8 m_pD3DTexture;
		D3DFORMAT chooseTextureFormat();
		bool hasTextureFormat(D3DFORMAT dFormat);
	};
}
#endif