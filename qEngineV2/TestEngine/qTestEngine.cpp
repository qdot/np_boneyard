#include "qtestengine.h"

qTestEngine* qTestEngine::m_pThis = NULL;

qTestEngine::qTestEngine(void)
{
	if(m_pThis == NULL)
	{
		m_pThis = this;
	}
	else
	{
		return;
	}
}

qTestEngine::~qTestEngine(void)
{
	m_pThis = NULL;
	m_spConsole->setMessage(MSG_SHUTDOWN, "qTestEngine", "Deconstructor : Object deconstructor called.");
	shutdown();
}

bool qTestEngine::init()
{
    m_spConsole.reset(new qEngine::qConsole());
	m_spConsole->init();
	m_spConsole->setMessage(MSG_INIT, "qTestEngine", "init : Object and Console initialized.");

	m_spConfig.reset(new qEngine::qConfigManager(m_spConsole));
	if(!m_spConfig->loadValuesFromFile("console.qe"))
	{
		m_spConsole->setMessage(MSG_ERROR, "qTestEngine", "init : CANNOT CREATE SETTINGS. FATAL ERROR.");
		return false;
	}

	m_spD3D.reset(new qEngine::qDirect3D(m_spConsole));
	if(!m_spD3D->init(m_spConfig))
	{
		m_spConsole->setMessage(MSG_ERROR, "qTestEngine", "init : CANNOT CREATE D3D APPLICATION. FATAL ERROR.");
		return false;
	}
	

	m_spTimer.reset(new qEngine::qTimer());
	m_spTimer->Init();

	m_spInput.reset(new qEngine::qInput());
	if(!m_spInput->init(m_spD3D->gethWnd(), m_spConsole, m_spConfig, Q_KEYBOARD | Q_MOUSE))
	{
		m_spConsole->setMessage(MSG_ERROR, "qTestEngine", "init : CANNOT ACQUIRE INPUT DEVICES. FATAL ERROR.");
		return false;
	}
	m_spInput->updateDevices();

	m_spSceneGraph.reset(new qEngine::qSceneGraph());
	m_spSceneGraph->init(m_spConfig);
	m_spSceneGraph->set2D();

    m_spStateManager.reset(new qEngine::qStateManager());
	m_spStateManager->push(render);
	
	m_spProcessManager.reset(new qEngine::qProcessManager());

	m_spTU.reset(new qEngine::qTestUtility(m_spConsole));

	m_spRenderer.reset(new qEngine::qRenderer(m_spConsole));

	m_spRenderer->loadTexture("logo.bmp");
	
	m_spRenderer->createStaticVB(m_spVB, D3DFVF_LitTexturedVertex, 24);

	m_spGUI.reset(new qEngine::qGUI(m_spRenderer));
	m_spGUI->init(m_spConfig);

	m_spGUI->loadCursor("cursor3.tga");
	m_spGUI->setCursor("cursor3.tga");
	m_spGUI->showCursor(true);

	m_spGUI->loadFont("font.tga");
	m_spGUI->setFont("font.tga");

	loadTexturedCube();

	//Render stage sets
	//Turn on ambient lighting 
	m_spRenderer->setRenderState( D3DRS_LIGHTING, FALSE );

	// Make it so that it'll draw triangles facing either towards or away from the camera
	m_spRenderer->setRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	// Turn on depth testing, just 'cause, well, it's nice
	m_spRenderer->setRenderState( D3DRS_ZENABLE,D3DZB_TRUE );

	// Set Alpha Blending
	m_spRenderer->setRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
	m_spRenderer->setRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );
	
	//Set Texture stage filtering so things look good
	m_spRenderer->setTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_LINEAR);
	m_spRenderer->setTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
	m_spRenderer->setTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	m_spConsole->setMessage(MSG_INIT, "qTestEngine", "init : TestEngine Initialized");
	return true;
}

void qTestEngine::gameLoop()
{
	while(m_spD3D->isAlive()  && m_spStateManager->numFunctions() > 0)
	{
		m_spD3D->handleMessages();
		m_spProcessManager->process(FRAME_PURPOSE);
		m_spStateManager->process(FRAME_PURPOSE);
	}
}

bool qTestEngine::render(int iPurpose)
{
	m_pThis->m_spInput->updateDevices();
	m_pThis->m_spGUI->clearText();
	if(m_pThis->m_spInput->keyDown(DIK_Q))
	{
		m_pThis->m_spD3D->setDead();
		return false;
	}

	if(FAILED(m_pThis->m_spRenderer->clearScreen()))
	{
		m_pThis->m_spConsole->setMessage(MSG_ERROR, "qTestEngine", "render : CANNOT CLEAR BACK BUFFER. FATAL ERROR.");
		m_pThis->m_spD3D->setDead();
		return false;
	}
	
	long _x, _y;
	m_pThis->m_spInput->getMousePosition(&_x, &_y);
	m_pThis->m_spGUI->setCursorPos((int) _x, (int) _y);

	m_pThis->m_spRenderer->setCamera();
	m_pThis->m_spRenderer->beginScene();

	
	m_pThis->m_spSceneGraph->set3D();
	m_pThis->m_spRenderer->setRenderState( D3DRS_ALPHABLENDENABLE, false );
	m_pThis->m_spRenderer->setTexture("logo.bmp", 0);
	m_pThis->m_spRenderer->renderVB(m_pThis->m_spVB);
	
	m_pThis->m_spSceneGraph->set2D();
	m_pThis->m_spGUI->addText("qEngine V1.0", 0, 0, 0xffffffff, 1.0f);
	m_pThis->m_spGUI->addText("FPS:" + boost::lexical_cast<string>(m_pThis->m_spTimer->GetFPS()), 0.0f, 32.0f, 0xffffffff, 1.0f);
	m_pThis->m_spGUI->renderGUI();
	
	m_pThis->m_spRenderer->endScene();

	if(m_pThis->m_spD3D->isAlive())
	{
		HRESULT hr = m_pThis->m_spRenderer->presentBuffer();
		if(hr == D3DERR_DEVICELOST)
		{
			m_pThis->m_spConsole->setMessage(MSG_ERROR, "qTestEngine", "render : CANNOT PRESENT BACK BUFFER DUE TO LOST DEVICE! FATAL ERROR.");	
			m_pThis->m_spD3D->setDead();
			return false;
		}
	}
	return true;
}

void qTestEngine::shutdown()
{
	m_spConsole->setMessage(MSG_SHUTDOWN, "qTestEngine", "Shutdown : Object shutting down.");
	m_spGUI.reset();
	m_spVB.reset();
	m_spTU.reset();
	m_spRenderer.reset();
	m_spProcessManager.reset();
	m_spStateManager.reset();
	m_spConfig.reset();
	m_spInput.reset();
	m_spD3D.reset();
	m_spTimer.reset();
	m_spConsole->writeMessagesToFile("TestConsole.qe");
	m_spConsole.reset();
}

void qTestEngine::loadTexturedCube()
{
	LITTEX_VERTEX* pVertex;

	UINT start = 0;
	
	pVertex = m_spVB->Lock(0, start);
	
	//front
	pVertex[0].pos = D3DXVECTOR3(-.5, -.5, .5);
	pVertex[0].norm = D3DXVECTOR3(0, 0, 1.0);
	pVertex[0].color = 0x00000fff;
	pVertex[1].pos = D3DXVECTOR3(-.5, .5, .5);
	pVertex[1].norm = D3DXVECTOR3(0, 0, 1.0);
	pVertex[1].color = 0x000000ff;
	pVertex[2].pos = D3DXVECTOR3(.5, -.5, .5);
	pVertex[2].norm = D3DXVECTOR3(0, 0, 1.0);
	pVertex[2].color = 0x0000f00f;
	pVertex[3].pos = D3DXVECTOR3(.5, .5, .5);
	pVertex[3].norm = D3DXVECTOR3(0, 0, 1.0);
	pVertex[3].color = 0x00f00ff0;
	pVertex[0].tu = 0.0;
	pVertex[0].tv = 0.0;
	pVertex[1].tu = 0.0;
	pVertex[1].tv = 1.0;
	pVertex[2].tu = 1.0;
	pVertex[2].tv = 0.0;
	pVertex[3].tu = 1.0;
	pVertex[3].tv = 1.0;
	
	//back
	pVertex[4].pos = D3DXVECTOR3(-.5, -.5, -.5);
	pVertex[4].norm = D3DXVECTOR3(0, 0, -1.0);
	pVertex[4].color = 0x00f00ff0;
	pVertex[5].pos = D3DXVECTOR3(-.5, .5, -.5);
	pVertex[5].norm = D3DXVECTOR3(0, 0, -1.0);
	pVertex[5].color = 0x00f00ff0;
	pVertex[6].pos = D3DXVECTOR3(.5, -.5, -.5);
	pVertex[6].norm = D3DXVECTOR3(0, 0, -1.0);
	pVertex[6].color = 0x00f00ff0;
	pVertex[7].pos = D3DXVECTOR3(.5, .5, -.5);
	pVertex[7].norm = D3DXVECTOR3(0, 0, -1.0);
	pVertex[7].color = 0x00f00ff0;
	pVertex[4].tu = 0.0;
	pVertex[4].tv = 0.0;
	pVertex[5].tu = 0.0;
	pVertex[5].tv = 1.0;
	pVertex[6].tu = 1.0;
	pVertex[6].tv = 0.0;
	pVertex[7].tu = 1.0;
	pVertex[7].tv = 1.0;

	//top
	pVertex[8].pos = D3DXVECTOR3(-.5, .5, -.5);
	pVertex[8].norm = D3DXVECTOR3(0, 1.0, 0);
	pVertex[8].color = 0x000f00ff;
	pVertex[9].pos = D3DXVECTOR3(-.5, .5, .5);
	pVertex[9].norm = D3DXVECTOR3(0, 1.0, 0);
	pVertex[9].color = 0x000f00ff;
	pVertex[10].pos = D3DXVECTOR3(.5, .5, -.5);
	pVertex[10].norm = D3DXVECTOR3(0, 1.0, 0);
	pVertex[10].color = 0x000f00ff;
	pVertex[11].pos = D3DXVECTOR3(.5, .5, .5);
	pVertex[11].norm = D3DXVECTOR3(0, 1.0, 0);
	pVertex[11].color = 0x000f00ff;
	pVertex[8].tu = 0.0;
	pVertex[8].tv = 0.0;
	pVertex[9].tu = 0.0;
	pVertex[9].tv = 1.0;
	pVertex[10].tu = 1.0;
	pVertex[10].tv = 0.0;
	pVertex[11].tu = 1.0;
	pVertex[11].tv = 1.0;

	//bottom
	pVertex[12].pos = D3DXVECTOR3(-.5, -.5, -.5);
	pVertex[12].norm = D3DXVECTOR3(0, -1.0, 0);
	pVertex[12].color = 0x0000f00f;
	pVertex[13].pos = D3DXVECTOR3(-.5, -.5, .5);
	pVertex[13].norm = D3DXVECTOR3(0, -1.0, 0);
	pVertex[13].color = 0x0000f00f;
	pVertex[14].pos = D3DXVECTOR3(.5, -.5, -.5);
	pVertex[14].norm = D3DXVECTOR3(0, -1.0, 0);
	pVertex[14].color = 0x0000f00f;
	pVertex[15].pos = D3DXVECTOR3(.5, -.5, .5);
	pVertex[15].norm = D3DXVECTOR3(0, -1.0, 0);
	pVertex[15].color = 0x0000f00f;
	pVertex[12].tu = 0.0;
	pVertex[12].tv = 0.0;
	pVertex[13].tu = 0.0;
	pVertex[13].tv = 1.0;
	pVertex[14].tu = 1.0;
	pVertex[14].tv = 0.0;
	pVertex[15].tu = 1.0;
	pVertex[15].tv = 1.0;

	//left
	pVertex[16].pos = D3DXVECTOR3(.5, -.5, -.5);
	pVertex[16].norm = D3DXVECTOR3(-1.0, 0, 0);
	pVertex[16].color = 0x0000ffff;
	pVertex[17].pos = D3DXVECTOR3(.5, -.5, .5);
	pVertex[17].norm = D3DXVECTOR3(-1.0, 0, 0);
	pVertex[17].color = 0x0000ffff;
	pVertex[18].pos = D3DXVECTOR3(.5, .5, -.5);
	pVertex[18].norm = D3DXVECTOR3(-1.0, 0, 0);
	pVertex[18].color = 0x0000ffff;
	pVertex[19].pos = D3DXVECTOR3(.5, .5, .5);
	pVertex[19].norm = D3DXVECTOR3(-1.0, 0, 0);
	pVertex[19].color = 0x0000ffff;
	pVertex[16].tu = 0.0;
	pVertex[16].tv = 0.0;
	pVertex[17].tu = 0.0;
	pVertex[17].tv = 1.0;
	pVertex[18].tu = 1.0;
	pVertex[18].tv = 0.0;
	pVertex[19].tu = 1.0;
	pVertex[19].tv = 1.0;

	//right
	pVertex[20].pos = D3DXVECTOR3(-.5, -.5, -.5);
	pVertex[20].norm = D3DXVECTOR3(1.0, 0, 0);
	pVertex[20].color = 0x000fffff;
	pVertex[21].pos = D3DXVECTOR3(-.5, -.5, .5);
	pVertex[21].norm = D3DXVECTOR3(1.0, 0, 0);
	pVertex[21].color = 0x000fffff;
	pVertex[22].pos = D3DXVECTOR3(-.5, .5, -.5);
	pVertex[22].norm = D3DXVECTOR3(1.0, 0, 0);
	pVertex[22].color = 0x000fffff;
	pVertex[23].pos = D3DXVECTOR3(-.5, .5, .5);
	pVertex[23].norm = D3DXVECTOR3(1.0, 0, 0);
	pVertex[23].color = 0x000fffff;
	pVertex[20].tu = 0.0;
	pVertex[20].tv = 0.0;
	pVertex[21].tu = 0.0;
	pVertex[21].tv = 1.0;
	pVertex[22].tu = 1.0;
	pVertex[22].tv = 0.0;
	pVertex[23].tu = 1.0;
	pVertex[23].tv = 1.0;


	m_spVB->Unlock();
}
