#pragma once
#include <qEngine/qCommonInclude.h>
#include "qresource.h"

namespace qEngine
{

	class qXFile :
		public qResource
	{

	public:

		struct sMeshInfo 
		{
			LPD3DXMESH lpD3DXMesh;
			D3DMATERIAL8* pD3DMaterialArray;
			string* pstrTextureArray;
			DWORD dwNumMaterials;
		};

		qXFile(void);
		~qXFile(void);
		bool loadResource(string strName);
		sMeshInfo getResource() {return m_sMeshInfo;}
		bool unloadResource();
	private:
		sMeshInfo m_sMeshInfo;
	};
}