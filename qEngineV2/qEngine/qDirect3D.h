#pragma once
#ifndef Q_D3D
#define Q_D3D

#include <qEngine/qCommonInclude.h>
#include <qEngine/qConfigManager.h>
#include <qEngine/qConsole.h>
#include <qEngine/qWindow.h>

namespace qEngine
{

	class qDirect3D : public qWindow
	{
	public:
		qDirect3D() {};
		qDirect3D(boost::shared_ptr< qConsole > spConsole);
		~qDirect3D(void);

		LPDIRECT3DDEVICE8 getDevice() { return m_lpD3DDevice; }
		D3DPRESENT_PARAMETERS getDeviceProps() { return m_d3dpp; }

		void getValuesFromConfigManager(boost::shared_ptr<qConfigManager> spCM);
		bool init(boost::shared_ptr<qConfigManager> spCM);
		void shutdown();

		UINT getWindowWidth();
		UINT getWindowHeight();
		UINT getWindowBPP();

		LPDIRECT3D8 m_pD3D;
		D3DDISPLAYMODE m_d3ddm;
		D3DPRESENT_PARAMETERS m_d3dpp; 
		static LPDIRECT3DDEVICE8 m_lpD3DDevice;

	private:
		UINT m_iAdapterID; //ID of the current adapter being used
		UINT m_iRenderer; //ID of the renderer being used (Software/HAL)
		UINT m_iResolutionMode; //ID of the renderer mode being used

		boost::shared_ptr< qConsole > m_spConsole;
		boost::shared_ptr< qWindow > m_spWindow;
	};
	
}

#endif