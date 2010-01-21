/************************************************************************
Class Name: qIFunctionManager
Date: 10/30/02
Purpose: This is the interface for the Function Manager classes, which
are StateManager and ProcessManager. The only thing that differs between
the two classes is the process function, so I decided to just derive
them off this interface.
************************************************************************/

/************************************************************************/
/* Includes                                                             */
/************************************************************************/

#pragma once

#define INIT_PURPOSE 0
#define FRAME_PURPOSE 1
#define SHUTDOWN_PURPOSE 2

#include <qEngine/qCommonInclude.h>

/************************************************************************/
/* Class Declarations                                                   */
/************************************************************************/

namespace qEngine
{
	typedef bool (*qFunctionPtr)(int Purpose);

	class qIFunctionManager
	{
	public:
		qIFunctionManager(void);
		virtual ~qIFunctionManager(void);
		void push(qFunctionPtr fpFunctionHandler);
		void pop();
		void clear();
		UINT numFunctions() { return (UINT)m_vFunctionVector.size(); }
		virtual void process(int iPurpose) = 0;
	protected:
		vector<qFunctionPtr> m_vFunctionVector;
	};
}