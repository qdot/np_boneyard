#include "qxfile.h"

namespace qEngine
{
	qXFile::qXFile(void)
	{
		m_sMeshInfo.lpD3DXMesh = NULL;
		m_sMeshInfo.pD3DMaterialArray = NULL;
		m_sMeshInfo.pstrTextureArray = NULL;
	}

	bool qXFile::loadResource(string strName)
	{
		LPD3DXBUFFER lpD3DXBuffer = NULL;
		if(FAILED(D3DXLoadMeshFromX((LPSTR)strName.c_str(), D3DXMESH_SYSTEMMEM, g_lpD3DDevice, NULL, &lpD3DXBuffer, &m_sMeshInfo.dwNumMaterials, &m_sMeshInfo.lpD3DXMesh)))
		{
			return false;
		}
		D3DXMATERIAL* tmpMat;
		tmpMat = (D3DXMATERIAL*)lpD3DXBuffer->GetBufferPointer();
		if(tmpMat != NULL)
		{
			m_sMeshInfo.pD3DMaterialArray = new D3DMATERIAL8[m_sMeshInfo.dwNumMaterials];
			m_sMeshInfo.pstrTextureArray = new string[m_sMeshInfo.dwNumMaterials];

			for(DWORD i=0; i < m_sMeshInfo.dwNumMaterials; i++)
			{
				m_sMeshInfo.pD3DMaterialArray[i] = tmpMat[i].MatD3D;
				m_sMeshInfo.pD3DMaterialArray[i].Ambient = m_sMeshInfo.pD3DMaterialArray[i].Diffuse;
				if(tmpMat[i].pTextureFilename != NULL)
				{
					m_sMeshInfo.pstrTextureArray[i] = tmpMat[i].pTextureFilename;
				}
			}
		}
		else
		{
			m_sMeshInfo.dwNumMaterials = 1;
			m_sMeshInfo.pD3DMaterialArray = new D3DMATERIAL8[1];
			m_sMeshInfo.pD3DMaterialArray[0].Diffuse.r = 1.0f;
			m_sMeshInfo.pD3DMaterialArray[0].Diffuse.g = 1.0f;
			m_sMeshInfo.pD3DMaterialArray[0].Diffuse.b = 1.0f;
			m_sMeshInfo.pD3DMaterialArray[0].Diffuse.a = 1.0f;
			m_sMeshInfo.pD3DMaterialArray[0].Ambient = m_sMeshInfo.pD3DMaterialArray[0].Diffuse;
		}
		return true;
	}

	bool qXFile::unloadResource()
	{	
		if(m_sMeshInfo.lpD3DXMesh)
		{
			m_sMeshInfo.lpD3DXMesh->Release();
		}
		
		return true;
	}

	qXFile::~qXFile(void)
	{
		unloadResource();
	}

}