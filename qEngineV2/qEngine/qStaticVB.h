#pragma once
#include "qresource.h"
#include <qEngine/StaticVB.h>

namespace qEngine
{
	class qStaticVB :
		public qResource
	{
	public:
		qStaticVB(void);
		~qStaticVB(void);
	};
}