#pragma once

#include <qEngine/qCommonInclude.h>
#include <qEngine/qConsole.h>
#include <qEngine/qConfigManager.h>
#include <qEngine/qDirect3D.h>
#include <qEngine/qStateManager.h>
#include <qEngine/qProcessManager.h>
#include <qEngine/qInput.h>
#include <qEngine/qTestUtility.h>
#include <qEngine/qTimer.h>
#include <qEngine/qRenderer.h>
#include <qEngine/qVertex.h>
#include <qEngine/StaticVB.h>
#include <qEngine/qGUI.h>
#include <qEngine/qSceneGraph.h>

class qTestEngine
{
public:
	qTestEngine(void);
	~qTestEngine(void);
	bool init();
	void shutdown();
	void gameLoop();
	static qTestEngine* m_pThis;
	static bool render(int iPurpose);
	void loadTexturedCube();
private:
	boost::shared_ptr< qEngine::qConsole > m_spConsole;
	boost::shared_ptr< qEngine::qConfigManager > m_spConfig;
	boost::shared_ptr< qEngine::qDirect3D > m_spD3D;
	boost::shared_ptr< qEngine::qStateManager > m_spStateManager;
	boost::shared_ptr< qEngine::qProcessManager > m_spProcessManager;
	boost::shared_ptr< qEngine::qTestUtility > m_spTU;
	boost::shared_ptr< qEngine::qInput > m_spInput;
	boost::shared_ptr< qEngine::qTimer > m_spTimer;
	boost::shared_ptr< qEngine::qRenderer > m_spRenderer;
	boost::shared_ptr< StaticVB< LITTEX_VERTEX > > m_spVB;
	boost::shared_ptr< qEngine::qSceneGraph > m_spSceneGraph;
	boost::shared_ptr< qEngine::qGUI > m_spGUI;
};
