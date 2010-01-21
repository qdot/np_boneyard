#ifndef Q_RENDERER
#define Q_RENDERER

#pragma once

#include <qEngine/qCommonInclude.h>
#include <qEngine/qConsole.h>
#include <qEngine/qResources.h>
#include <qEngine/qResourceManager.h>
#include <qEngine/StaticVB.h>
#include <qEngine/DynamicVB.h>
#include <qEngine/StaticIB.h>
#include <qEngine/DynamicIB.h>

namespace qEngine
{
	class qRenderer
	{
	public:
		//Constructor/Destructor
		qRenderer(void);
		qRenderer(boost::shared_ptr<qConsole> m_spConsole);
		~qRenderer(void);

		//Functions that simple wrap the device calls so the actual engine doesn't 
		//touch the device
		HRESULT clearScreen();
		HRESULT presentBuffer();

		//Texturing Functions
		bool loadTexture(string strName);
		bool setTexture(string strName, int iStage);
		bool unloadTexture(string strName);

		//Mesh Functions
		bool loadMesh(string strName);
		bool drawMesh(string strName);
		bool unloadMesh(string strName);


		/*
		Sidenote: Since the nVidia index and vertex buffer classes are templated,
		we can't create a good resource manager class for them here. Therefore it 
		is up to the client program to deal with vertex and index buffers as it
		needs them. All draw functions therefore have VB and IB parameters in 
		which user created buffers can be passed. However, all creation is dealt
		with inside the engine so the outside client program does not have to 
		see the D3DDevice (even though it does have access to it since it is 
		declared in the qEngine namespace as global).
		*/

		//Vertex Buffer Functions
		template <class VertexType>
		void createStaticVB(boost::shared_ptr< StaticVB< VertexType > >& spVB, const DWORD& theFVF, const unsigned int& theVertexCount)
		{
			spVB.reset(new StaticVB<VertexType>(g_lpD3DDevice, theFVF, theVertexCount));
		}
		template <class VertexType>
		void createDynamicVB(boost::shared_ptr< DynamicVB< VertexType > >& spVB, const DWORD& theFVF, const unsigned int& theVertexCount)
		{
			spVB.reset(new DynamicVB<VertexType>(g_lpD3DDevice, theFVF, theVertexCount));
		}
		template <class VertexType>
		bool renderVB(boost::shared_ptr< StaticVB< VertexType > >& spVB)
		{

			g_lpD3DDevice->SetStreamSource(0, spVB->GetInterface(), sizeof(VertexType));
			g_lpD3DDevice->SetVertexShader(spVB->getFVF());
			for(int i = 0; i < 6; i++)
			{
				g_lpD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4*i, 2);
			}
			return true;
		}
		template <class VertexType>
		bool renderVB(boost::shared_ptr< DynamicVB< VertexType > >& spVB)
		{

			g_lpD3DDevice->SetStreamSource(0, spVB->GetInterface(), sizeof(VertexType));
			g_lpD3DDevice->SetVertexShader(spVB->getFVF());
			for(int i = 0; i < 6; i++)
			{
				g_lpD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4*i, 2);
			}
			return true;
		}

		//Index Buffer Functions
		template <class IndexType>
		void createStaticIB(boost::shared_ptr< StaticIB< IndexType > >& spIB, const unsigned int& theIndexCount )
		{
			spIB.reset(new StaticIB<IndexType>(g_lpD3DDevice, theIndexCount));
		}
		template <class IndexType>
		void createDynamicIB(boost::shared_ptr< DynamicIB< IndexType > >& spIB, const unsigned int& theIndexCount )
		{
			spIB.reset(new DynamicIB<IndexType>(g_lpD3DDevice, theIndexCount));
		}

		//Scene Functions
		bool beginScene() { return SUCCEEDED(g_lpD3DDevice->BeginScene()); }
		bool endScene()  { return SUCCEEDED(g_lpD3DDevice->EndScene()); }

		//Misc Functions
		void setCamera();
		bool setRenderState(const D3DRENDERSTATETYPE& state, const DWORD& value);
		bool setTextureStageState(const DWORD& stage, const D3DTEXTURESTAGESTATETYPE& state, const DWORD& value);

	private:
		boost::shared_ptr< qResourceManager< qTexture > > m_rmTextureMgr;
		boost::shared_ptr< qResourceManager< qXFile > > m_rmXFileMgr;
		boost::shared_ptr< qConsole > m_spConsole;
	};
}
#endif
