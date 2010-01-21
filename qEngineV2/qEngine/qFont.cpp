#include "qfont.h"

namespace qEngine
{

	qFont::qFont(void)
	{
	}

	bool qFont::loadResource(string strName)
	{
		string strINIFile = strName.substr(0, (strName.length() - 4));
		strINIFile += ".ini";
		
		if(m_sFontInfo.s_CMFontWidth.loadValuesFromFile(strINIFile))
		{
			m_sFontInfo.s_strFilename = strName;
			return true;
		}
		return false;
	}

	bool qFont::unloadResource()
	{
		m_sFontInfo.s_CMFontWidth.clearValues();
		return true;
	}

	qFont::~qFont(void)
	{
	}

}