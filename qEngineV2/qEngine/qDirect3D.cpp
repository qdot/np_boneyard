#include "qdirect3d.h"

namespace qEngine
{
	LPDIRECT3DDEVICE8 qDirect3D::m_lpD3DDevice = NULL;
	LPDIRECT3DDEVICE8 g_lpD3DDevice = qDirect3D::m_lpD3DDevice;
	
	/********************************* qDirect3D::qDirect3D **********************************
	description:
	Constructor

	@spConsole[in]:
	Pointer to an already initialized console

	access: public
	------------------------------------------------------------------------------------------
	created: 23.10.02 - 23:46:51
	creator: qDot
	*****************************************************************************************/
	qDirect3D::qDirect3D(boost::shared_ptr<qConsole> spConsole) : qWindow(spConsole), m_spConsole(spConsole)
	{
	}
	
	/******************************** qDirect3D::~qDirect3D **********************************
	description:
	Destructor. Calls shutdown().

	access: public, state: virtual
	------------------------------------------------------------------------------------------
	created: 23.10.02 - 23:47:08
	creator: qDot
	*****************************************************************************************/

	qDirect3D::~qDirect3D(void)
	{
		shutdown();
	}

	/*********************************** qDirect3D::init *************************************
	description:
	Initialization function. Creates a Win32 window, then does Direct3D initialization.

	@spCM[in]:
	Smart pointer to a config manager that has been loaded with the engine settings
	file.

	value:
	Returns true if window is created successfully, false otherwise.

	access: public
	------------------------------------------------------------------------------------------
	created: 23.10.02 - 23:48:19
	creator: qDot
	*****************************************************************************************/

	bool qDirect3D::init(boost::shared_ptr<qConfigManager> spCM)
	{

		//Get Values from Configuration Manager for both the parents and child objects
		qWindow::getValuesFromConfigManager(spCM);
		getValuesFromConfigManager(spCM);

		g_lpD3DDevice = NULL;

		m_spConsole->setMessage(MSG_INIT, "qDirect3D", "init : Creating Win32 window");
		
		//Call the initializer function to make sure the window is created.
		if(!qWindow::init())
		{
			m_spConsole->setMessage(MSG_ERROR, "qDirect3D", "init : Window creation failed");
			m_eStatus = WIN_ERROR;
			return false;
		}
		m_spConsole->setMessage(MSG_INIT, "qDirect3D", "init : Window creation successfully");

		// create a Direct3D device
		m_pD3D = Direct3DCreate8( D3D_SDK_VERSION );
		if(m_pD3D == NULL) {
			m_spConsole->setMessage(MSG_ERROR, "qDirect3D", "init : Direct3DCreate8 failed");
			m_eStatus = WIN_ERROR;
			return false;
		}

		// Get the current desktop display mode
		if(FAILED(m_pD3D->GetAdapterDisplayMode(m_iAdapterID, &m_d3ddm))) 
		{
			m_spConsole->setMessage(MSG_ERROR, "qDirect3D", "init : GetAdapterDisplayMode failed");
			m_eStatus = WIN_ERROR;
			return false;
		}
		ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) ); // this will set all members to zero

		D3DDISPLAYMODE _mode;
		m_pD3D->EnumAdapterModes(m_iAdapterID, m_iResolutionMode, &_mode);

		//Pull information from mode selected in setup

		if(m_bWindowedMode)
		{
			//Settings for Windowed Mode
			m_d3dpp.Windowed = TRUE;
			m_d3dpp.BackBufferFormat = m_d3ddm.Format;
			m_d3dpp.BackBufferWidth = 0;
			m_d3dpp.BackBufferHeight = 0;
		}
		else
		{
			//Settings for Fullscreen Mode
			m_d3dpp.FullScreen_RefreshRateInHz = _mode.RefreshRate;
			m_d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

			m_d3dpp.BackBufferFormat = _mode.Format;
			m_d3dpp.BackBufferWidth = _mode.Width;
			m_d3dpp.BackBufferHeight = _mode.Height;
			m_d3dpp.Windowed = FALSE;
		}

		//Settings used in both modes
		m_d3dpp.BackBufferCount = 1;
		m_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_d3dpp.hDeviceWindow = m_hWnd;

		//Have DirectX Handle Depth Buffers
		m_d3dpp.EnableAutoDepthStencil = true;
		m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		// no special flags
		m_d3dpp.Flags = 0;

		// Create the Direct3D device. 
		if(FAILED(m_pD3D->CreateDevice(m_iAdapterID, 
			D3DDEVTYPE_HAL, m_hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&m_d3dpp, &m_lpD3DDevice)))
		{
			m_spConsole->setMessage(MSG_ERROR, "qDirect3D", "init : CreateDevice failed");
			m_eStatus = WIN_ERROR;
			return false;
		}
		qEngine::g_lpD3DDevice = m_lpD3DDevice;
		m_spConsole->setMessage(MSG_INIT, "qDirect3D", "init : Direct3D Initialized");
		return true;
	}

	/********************************* qDirect3D::shutdown ***********************************
	description:
	Sets all COM pointers in the class to NULL and shuts down DirectX.

	access: public
	------------------------------------------------------------------------------------------
	created: 23.10.02 - 23:49:21
	creator: qDot
	*****************************************************************************************/
	
	void qDirect3D::shutdown(void)
	{
		if(m_lpD3DDevice != NULL) {
			m_lpD3DDevice->Release();
			m_lpD3DDevice = NULL;
			g_lpD3DDevice = NULL;
		}
		if(m_pD3D != NULL) {
			m_pD3D->Release();
			m_pD3D = NULL;
		}
		m_spConsole->setMessage(MSG_SHUTDOWN, "qDirect3D", "shutdown : Direct3D Shutdown Complete");
	}

	/************************ qDirect3D::getValuesFromConfigManager **************************
	description:
	Gets values from the config manager.

	@spCM[in]:
	Smart pointer to an initialized config manager which has the settings file loaded.

	access: public
	------------------------------------------------------------------------------------------
	created: 23.10.02 - 23:49:56
	creator: qDot
	*****************************************************************************************/

	void qDirect3D::getValuesFromConfigManager(boost::shared_ptr<qConfigManager> spCM)
	{
		//Values to get from Console:
		//m_iRenderer - Hardware or Software Rendering
		//m_iResolutionMode - Direct3D Resolution Mode
		//m_iAdapterID - ID for the adapter Direct3D should use

		m_iRenderer = boost::lexical_cast<UINT>(spCM->getValue("DXRenderer"));
		m_iResolutionMode = boost::lexical_cast<UINT>(spCM->getValue("DXResolutionMode"));
		m_iAdapterID = boost::lexical_cast<UINT>(spCM->getValue("DXAdapterID"));

	}
}