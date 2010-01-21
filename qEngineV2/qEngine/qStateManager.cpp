#include "qstatemanager.h"

namespace qEngine
{

/***************************** qStateManager::qStateManager ******************************
description:
   Constructor

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 16:56:13
creator: qDot
*****************************************************************************************/

	qStateManager::qStateManager(void)
	{
	}

/**************************** qStateManager::~qStateManager ******************************
description:
   Pops everything of the stack

access: public, state: virtual
------------------------------------------------------------------------------------------
created: 30.10.02 - 16:56:29
creator: qDot
*****************************************************************************************/

	qStateManager::~qStateManager(void)
	{
		while(m_vFunctionVector.size() > 0) pop();
	}

/******************************** qStateManager::process *********************************
description:
   Makes sure there is at least one process on the stack, and if so, runs whatever is on
   top.

@iPurpose[in]:
   Calling purpose to pass to the function

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 16:57:06
creator: qDot
*****************************************************************************************/

	void qStateManager::process(int iPurpose)
	{
		if(m_vFunctionVector.size() > 0)
		{
			m_vFunctionVector[m_vFunctionVector.size() - 1](iPurpose);
		}
	}
}
