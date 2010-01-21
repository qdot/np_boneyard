#ifndef Q_COMMON_FUNCTIONS
#define Q_COMMON_FUNCTIONS 1

/* C includes for time retrival */
#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>

namespace qEngine
{
	char* returnCurrentTime()
	{
		//TODO: Figure out why 
		struct __timeb64 timebuffer;
		char *timeline;
		char *timeOutput = NULL;
		_ftime64( &timebuffer );
		timeline = _ctime64( & ( timebuffer.time ) );
		sprintf(timeOutput, "%.19s.%hu %s", timeline, timebuffer.millitm, &timeline[20] );
		return timeOutput;
	}
}

#endif