#pragma once
#include <qEngine/qCommonInclude.h>
#include <qEngine/qConfigManager.h>
#include <qEngine/qResource.h>

namespace qEngine
{
	class qFont : public qResource
	{
	public:
		
		struct sFontInfo
		{
			string s_strFilename;
			qConfigManager s_CMFontWidth;
		};

		qFont(void);
		~qFont(void);
		bool loadResource(string strName);
		sFontInfo getResource() {return m_sFontInfo;}
		bool unloadResource();
	private:
		sFontInfo m_sFontInfo;
	};
}