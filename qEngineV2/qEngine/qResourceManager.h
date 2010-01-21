/********************************************************************
	created:	2002/11/05
	created:	5:11:2002   2:41
	filename: 	c:\documents and settings\qdot\my documents\gameproject\qenginev2\qengine\qresourcemanager.h
	file path:	c:\documents and settings\qdot\my documents\gameproject\qenginev2\qengine
	file base:	qresourcemanager
	file ext:	h
	author:		qDot <qDot@numberporn.com>
	purpose:	Resource loader class. This is just a wrapper for a 
	map which relates a string to some resource type. We assume all resources will 
	come from files, and template for the type of resource it is. We
	also assume that the resource will be derived from the resource
	template, so all functions are assumed to be filled in, since
	the resource template itself is pure virtual.
*********************************************************************/
#pragma once
#ifndef Q_RESOURCEMGR
#define Q_RESOURCEMGR
#include <qEngine/qCommonInclude.h>
#include <qEngine/qConsole.h>
#include <qEngine/qResource.h>


namespace qEngine
{
	template <typename RESOURCE>
	class qResourceManager
	{
	typedef map<string, RESOURCE*> tResourceMap;
	typedef typename tResourceMap::iterator tResourceItr;
	public:
		qResourceManager(boost::shared_ptr<qConsole> spConsole, string strResourceName); 
		~qResourceManager(void);
		RESOURCE* getResource(string strName);
		bool hasResource(string strName);
		bool deleteResource(string strName);
	private:
		bool addResource(string strName);
		qResourceManager(void) {}
		string m_strResourceName;
		tResourceMap m_mResourceMap;
		boost::shared_ptr<qConsole> m_spConsole;
	};

	template <typename RESOURCE>
	qResourceManager<RESOURCE>::qResourceManager(boost::shared_ptr<qConsole> spConsole, string strResourceName) : m_spConsole(spConsole), m_strResourceName(strResourceName)
	{
		m_spConsole->setMessage(MSG_COMMON, m_strResourceName, "Constructor : Constructor called");
	}

	template <typename RESOURCE>
	qResourceManager<RESOURCE>::~qResourceManager()
	{	
		m_spConsole->setMessage(MSG_COMMON, m_strResourceName, "Destructor : Destructor called");
		for(tResourceItr i = m_mResourceMap.begin(); i != m_mResourceMap.end(); ++i)
		{
			i->second->unloadResource();
		}
	}

	template <typename RESOURCE>
	bool qResourceManager<RESOURCE>::addResource(string strName)
	{
		RESOURCE* tmpResource = new RESOURCE();
		if(tmpResource->loadResource(strName))
		{
			m_mResourceMap.insert(make_pair(strName, tmpResource));
			return true;
		}
		return false;
	}

	template <typename RESOURCE>
	RESOURCE* qResourceManager<RESOURCE>::getResource(string strName)
	{
		if(m_mResourceMap.find(strName) == m_mResourceMap.end())
		{
			if(!addResource(strName))
			{
				return NULL;
			}
		}			
		m_mResourceMap[strName]->incRefCount();
		return m_mResourceMap[strName];
	}

	template <typename RESOURCE>
	bool qResourceManager<RESOURCE>::hasResource(string strName)
	{
		if(m_mResourceMap.find(strName) != m_mResourceMap.end())
		{
			return true;
		}
		return false;
	}

	template <typename RESOURCE>
	bool qResourceManager<RESOURCE>::deleteResource(string strName)
	{
		if(hasResource(strName))
		{
			m_mResourceMap[strName]->decRefCount();
			if (m_mResourceMap[strName]->getRefCount() == 0)
			{
				m_mResourceMap[strName]->unloadResource();
			}
			return true;
		}
		return false;
	}
}

#endif
