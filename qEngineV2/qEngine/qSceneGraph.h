#pragma once

#include <qEngine/qCommonInclude.h>
#include <qEngine/qConfigManager.h>
#include <qEngine/qViewport.h>

namespace qEngine
{
	class qSceneGraph
	{
	public:
		qSceneGraph(void);
		~qSceneGraph(void);
		bool init(boost::shared_ptr<qConfigManager> spCM);
		void set2D();
		void set3D();
	private:
		bool is2D;
		boost::shared_ptr<qViewport> m_spViewport;
	};
}