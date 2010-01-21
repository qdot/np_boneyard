#pragma once
#ifndef Q_RESOURCE
#define Q_RESOURCE

#include <qEngine/qCommonInclude.h>

namespace qEngine
{
	class qResource
	{
	public:
		qResource(void) {}
		virtual ~qResource(void) {}
		virtual bool loadResource(string strName = "") = 0;
		virtual bool unloadResource() = 0;
		inline void incRefCount() { ++m_iReferenceCount; }
		inline void decRefCount() { --m_iReferenceCount; }
		inline int getRefCount() { return m_iReferenceCount; }
		inline string getName() { return m_strName; }
	protected:
		int m_iReferenceCount;
		string m_strName;
	};
}

#endif
