#pragma once
#include <qEngine/qCommonInclude.h>
#include <qEngine/qConfigManager.h>

namespace qEngine
{
	class qViewport
	{
	public:
		qViewport(void);
		~qViewport(void);
		inline void setScreenDimensions(float fWidth, float fHeight);
		void setOrthoViewport();
		void setOrthoViewport(float fNear, float fFar);
		void setOrthoViewport(float fWidth, float fHeight, float fNear, float fFar);
		void setProjViewport();
		void setProjViewport(float fNear, float fFar);
		void setProjViewport(float fWidth, float fHeight, float fNear, float fFar);
		void getValuesFromCM(boost::shared_ptr<qConfigManager> spCM);
		void setValuesInCM(boost::shared_ptr<qConfigManager> spCM);
	private:
		int m_fScreenWidth;
		int m_fScreenHeight;
		float m_fPOV;
		float m_fProjClipNear;
		float m_fProjClipFar;
		float m_fOrthoClipNear;
		float m_fOrthoClipFar;
	};
}