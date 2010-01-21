#include "qprocessmanager.h"

namespace qEngine
{

/*************************** qProcessManager::qProcessManager ****************************
description:
   Constructor

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 17:00:51
creator: qDot
*****************************************************************************************/

	qProcessManager::qProcessManager(void)
	{
	}

/************************** qProcessManager::~qProcessManager ****************************
description:
   Destructor

access: public, state: virtual
------------------------------------------------------------------------------------------
created: 30.10.02 - 17:01:02
creator: qDot
*****************************************************************************************/

	qProcessManager::~qProcessManager(void)
	{
		while(m_vFunctionVector.size() > 0) pop();
	}

/******************************* qProcessManager::process ********************************
description:
   Runs all functions in the function vector (running order is in the order the functions
   were added to the vector)

@iPurpose[in]:
   Calling purpose to send to the functions.

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 17:01:48
creator: qDot
*****************************************************************************************/

	void qProcessManager::process(int iPurpose)
	{
		for(UINT i = 0; i < m_vFunctionVector.size(); ++i)
		{
			m_vFunctionVector[i](iPurpose);
		}
	}
}