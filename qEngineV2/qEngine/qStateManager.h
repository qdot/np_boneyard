/************************************************************************
Class Name: qStateManager
Date: 10/30/02
Purpose: This is a derived class of the Function Manager. It maintains a
stack of function pointers, and runs whatever function is currently on 
the top of the stack.
************************************************************************/

/************************************************************************/
/* Includes                                                             */
/************************************************************************/

#pragma once
#include <qEngine/qCommonInclude.h>
#include <qEngine/qIFunctionManager.h>

/************************************************************************/
/* Class Declarations                                                   */
/************************************************************************/

namespace qEngine
{
	class qStateManager :
		public qIFunctionManager
	{
	public:
		qStateManager(void);
		~qStateManager(void);
		void process(int iPurpose);
	};

}