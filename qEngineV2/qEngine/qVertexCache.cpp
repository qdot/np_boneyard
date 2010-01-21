#include "qvertexcache.h"

namespace qEngine
{

	// qVertexCache.cpp: implementation of the CVertexCache class.
	//
	//////////////////////////////////////////////////////////////////////

	#include "qVertexCache.h"

	#define SAFE_RELEASE(x) if (x) {x->Release(); x=NULL; }

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	qVertexCache::qVertexCache(UINT maxVertices,UINT maxIndices,UINT stride,DWORD fvf,
				DWORD processing)
	{
		// create the vertex buffer
		m_vBuf=NULL;
		g_lpD3DDevice->CreateVertexBuffer(maxVertices*stride,
					D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY|processing,
					fvf,D3DPOOL_DEFAULT,&m_vBuf);

		// create the index buffer
		m_iBuf=NULL;
		g_lpD3DDevice->CreateIndexBuffer(maxIndices*sizeof(WORD),
					D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY|processing,
					D3DFMT_INDEX16,D3DPOOL_DEFAULT,&m_iBuf);

		// clear the vertex and index counters
		m_numVertices=0;
		m_numIndices=0;

		// save buffer sizes, vertex format, and stride
		m_maxVertices=maxVertices;
		m_maxIndices=maxIndices;
		m_stride=stride;
		m_fvf=fvf;

		// clear buffer pointers
		m_indPtr=NULL;
		m_vertPtr=NULL;
	}

	qVertexCache::~qVertexCache()
	{
		// release vertex and index buffers
		SAFE_RELEASE(m_vBuf);
		SAFE_RELEASE(m_iBuf);
	}

	HRESULT qVertexCache::drawCache(UINT numVertices, UINT numIndices, const WORD *pIndexData,
				const void *pVertexStreamZeroData)
	{
		HRESULT hr;

		// will this fit in the cache?
		if (m_numVertices+numVertices>m_maxVertices||
			m_numIndices+numIndices>m_maxIndices) 

			// no, flush the cache
			flushCache();

		// check to see if we have pointers into buffers, lock if needed
		if (!m_indPtr)
			if (FAILED(hr=m_iBuf->Lock(0,0,(BYTE **) &m_indPtr,D3DLOCK_DISCARD)))
				return hr;
		if (!m_vertPtr) 
			if (FAILED(hr=m_vBuf->Lock(0,0,&m_vertPtr,D3DLOCK_DISCARD)))
				return hr;

		// copy the vertices into the cache
		memcpy(&m_vertPtr[m_stride*m_numVertices],pVertexStreamZeroData,m_stride*numVertices);

		// save current index count
		int startInd=m_numVertices;

		// loop through the indices
		for (int i=0;i<numIndices;i++) {
			
			// add the index
			m_indPtr[m_numIndices]=((pIndexData!=NULL)?pIndexData[i]:i)+startInd;

			// increment the index counter
			m_numIndices++;
		}

		// adjust vertex counter
		m_numVertices+=numVertices;

		// return success
		return S_OK;
	}

	HRESULT qVertexCache::flushCache()
	{
		HRESULT hr;

		// unlock the vertex and index buffers
		if (m_indPtr) {
			m_iBuf->Unlock();
			m_indPtr=NULL;
		}
		if (m_vertPtr) {
			m_vBuf->Unlock();
			m_vertPtr=NULL;
		}

		// are there triangles to render?
		if (m_numIndices&&m_numVertices) 

			// yes, render them
			if (FAILED(hr=g_lpD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
								0,m_numVertices,
								0,m_numIndices/3)))
				return hr;

		// clear the vertex and index counters
		m_numVertices=0;
		m_numIndices=0;

		// return success
		return S_OK;
	}

	HRESULT qVertexCache::startCache()
	{
		HRESULT hr;

		// set the index buffer, vertex buffer, and shader for the device
		g_lpD3DDevice->SetIndices(m_iBuf,0);
		g_lpD3DDevice->SetStreamSource(0,m_vBuf,m_stride);
		g_lpD3DDevice->SetVertexShader(m_fvf);
		
		// clear the vertex and index counters
		m_numVertices=0;
		m_numIndices=0;

		// lock the vertex and index buffers
		m_indPtr=NULL;
		if (FAILED(hr=m_iBuf->Lock(0,0,(BYTE **) &m_indPtr,D3DLOCK_DISCARD)))
			return hr;
		m_vertPtr=NULL;
		if (FAILED(hr=m_vBuf->Lock(0,0,&m_vertPtr,D3DLOCK_DISCARD)))
			return hr;

		// return success
		return S_OK;
	}


}