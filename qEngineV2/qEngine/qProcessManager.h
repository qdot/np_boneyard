/************************************************************************
Class Name: ProcessManager
Date: 10/30/02
Purpose: This is a derived class of the Function Manager. It maintains a
vector of function pointers, and runs all functions on the stack every
time process() is called.
************************************************************************/

/************************************************************************/
/* Includes                                                             */
/************************************************************************/

#pragma once
#include "qifunctionmanager.h"

/************************************************************************/
/* Class Declarations                                                   */
/************************************************************************/

namespace qEngine
{

	class qProcessManager :
		public qIFunctionManager
	{
	public:
		qProcessManager(void);
		~qProcessManager(void);
		void process(int iPurpose);
	};

}