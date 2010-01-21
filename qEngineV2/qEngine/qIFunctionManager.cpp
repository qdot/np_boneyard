#include "qifunctionmanager.h"

namespace qEngine
{

/************************* qIFunctionManager::qIFunctionManager **************************
description:
   Constructor, reserves space for the function vectors

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 16:50:43
creator: qDot
*****************************************************************************************/

	qIFunctionManager::qIFunctionManager(void)
	{
		m_vFunctionVector.reserve(10);
	}

/************************ qIFunctionManager::~qIFunctionManager **************************
description:
   Destructor, clears the function vectors

access: public, state: virtual
------------------------------------------------------------------------------------------
created: 30.10.02 - 16:51:13
creator: qDot
*****************************************************************************************/

	qIFunctionManager::~qIFunctionManager(void)
	{
		clear();
	}

/******************************* qIFunctionManager::push *********************************
description:
   Adds a function to the stack

@fpFunctionHandler[in]:
   Pointer to a boolean function that takes an integer as a parameter.

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 16:52:04
creator: qDot
*****************************************************************************************/

	void qIFunctionManager::push(qFunctionPtr fpFunctionHandler)
	{
		m_vFunctionVector.push_back(fpFunctionHandler);
	}

/******************************** qIFunctionManager::pop *********************************
description:
   Pops the last function off the stack

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 16:52:26
creator: qDot
*****************************************************************************************/

	void qIFunctionManager::pop()
	{
		m_vFunctionVector.pop_back();
	}

/******************************* qIFunctionManager::clear ********************************
description:
   Clears the function vector

access: public
------------------------------------------------------------------------------------------
created: 30.10.02 - 16:52:39
creator: qDot
*****************************************************************************************/

	void qIFunctionManager::clear()
	{
		m_vFunctionVector.clear();
	}

}