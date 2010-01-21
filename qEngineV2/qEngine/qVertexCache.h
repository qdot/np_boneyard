/* Code adapted from http://www.mvps.org/directx/articles/vcache.htm */

#pragma once
#include <qEngine/qCommonInclude.h>
#include "qresource.h"

namespace qEngine
{
	class qVertexCache 
	{
	public:
		qVertexCache(UINT maxVertices,UINT maxIndices,UINT stride,DWORD fvf,DWORD processing);
		virtual ~qVertexCache();
		HRESULT flushCache();
		HRESULT startCache();
		HRESULT drawCache(UINT numVertices,UINT numIndices,const WORD *pIndexData,
				const void *pVertexStreamZeroData);
		bool loadResource(string strName = "");
		bool unloadResource();
	private:
		DWORD m_fvf;
		D3DPRIMITIVETYPE m_drawType;
		UINT m_maxVertices;
		UINT m_numVertices;
		UINT m_maxIndices;
		UINT m_numIndices;
		IDirect3DVertexBuffer8 *m_vBuf;
		IDirect3DIndexBuffer8 *m_iBuf;
		UINT m_stride;
		BYTE *m_vertPtr;
		WORD *m_indPtr;
	};
}