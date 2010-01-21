#include "qscenegraph.h"

namespace qEngine
{

	qSceneGraph::qSceneGraph(void) : is2D(false)
	{
		m_spViewport.reset(new qViewport());
	}

	qSceneGraph::~qSceneGraph(void)
	{
	}

	bool qSceneGraph::init(boost::shared_ptr<qConfigManager> spCM)
	{
		//Viewport Setup
		m_spViewport->getValuesFromCM(spCM);

		return true;
	}

	void qSceneGraph::set2D()
	{
		if (is2D) return;
		m_spViewport->setOrthoViewport();
	}

	void qSceneGraph::set3D()
	{
		if (!is2D) return;
		m_spViewport->setProjViewport();
	}


}