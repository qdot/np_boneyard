#pragma once

#include <qEngine/qCommonInclude.h>
#include <qEngine/qResourceManager.h>
#include <qEngine/qTextureResource.h>

namespace qEngine
{
	class qTextureManager : public qResourceManager
	{
		qTextureManager();
		~qTextureManager();
		void init();
		void shutdown();
		void addResource(string strName, string strFilename);
		virtual void deleteResource(string strName);
	};
}