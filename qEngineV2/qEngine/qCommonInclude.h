/********************************************************************
	created:	2002/11/21
	created:	21:11:2002   19:40
	filename: 	c:\documents and settings\qdot\my documents\gameproject\qenginev2\qengine\qcommoninclude.h
	file path:	c:\documents and settings\qdot\my documents\gameproject\qenginev2\qengine
	file base:	qcommoninclude
	file ext:	h
	author:		qDot <qDot@numberporn.com>
	
	purpose:	This file contains all of the common header includes
				for the qEngine library.
*********************************************************************/

#pragma once
#ifndef Q_COMMON
#define Q_COMMON

/* C includes */
#include <stdio.h>
#include <stdlib.h>

/* C++/STL includes */
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <cassert>

/* boost includes */
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/array.hpp>

/* DirectX/D3D includes */
#include <d3d8.h>
#include <d3dx8.h>
#include <dinput.h>

/* Using statement so the code isn't littered with std:: */
using namespace std;

//ONE GLOBAL, AND ONLY ONE GLOBAL. I'm just sick of passing the
//device everywhere. I'm sure you understand.

namespace qEngine
{
	extern LPDIRECT3DDEVICE8 g_lpD3DDevice;
}

//Various Utility Functions that don't fit in any particular class
bool parseQFile(string strFilename, map<string, string>* mValueMap);

#endif